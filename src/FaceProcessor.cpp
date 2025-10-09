#include "FaceProcessor.h"
#include "ImageProcessor.h"
#include <iostream>

FaceProcessor::FaceProcessor(const std::string& cascadePath,
    const std::string& lockscreenPath,
    const std::string& warningPath)
    : lockscreenPath(lockscreenPath), warningPath(warningPath)
{
    if (!face_cascade.load(cascadePath)) {
        throw std::runtime_error("Nepodařilo se načíst Haar cascade: " + cascadePath);
    }
}

cv::Point2f FaceProcessor::detect_face(const cv::Mat& frame) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

    cv::Point2f center(-1.0f, -1.0f);
    if (!faces.empty()) {
        cv::Rect face = faces[0];
        center.x = static_cast<float>(face.x + face.width / 2) / frame.cols;
        center.y = static_cast<float>(face.y + face.height / 2) / frame.rows;
    }
    return center;
}

int FaceProcessor::run_from_camera(FPSMeter* fps) {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Nepodarilo se otevrit kameru!\n";
        return EXIT_FAILURE;
    }

    cv::Mat frame;
    cv::Mat lockscreen = cv::imread(lockscreenPath);
    if (lockscreen.empty()) {
        std::cerr << "Nepodarilo se nacist lockscreen obrazek!\n";
        return EXIT_FAILURE;
    }

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::Point2f center = detect_face(frame);

        cv::Mat scene;
        if (center.x >= 0.0f && center.y >= 0.0f) {
            frame.copyTo(scene);
            CrossDrawer::draw_cross_normalized(scene, center, 30);
        }
        else {
            scene = lockscreen.clone();
        }

        cv::imshow("Face Detection", scene);

        if (fps) { // měření FPS
            fps->update();
            if (fps->is_updated())
                std::cout << "FPS: " << fps->get() << std::endl;
        }

        int key = cv::waitKey(30);
        if (key == 27) break; // ESC -> konec
    }

    cap.release();
    return EXIT_SUCCESS;
}

int FaceProcessor::run_from_camera_plus(FPSMeter* fps) {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Nepodarilo se otevrit kameru!\n";
        return EXIT_FAILURE;
    }

    cv::Mat frame;
    cv::Mat lockscreen = cv::imread(lockscreenPath);
    cv::Mat warning = cv::imread(warningPath);
    if (lockscreen.empty() || warning.empty()) {
        std::cerr << "Nepodarilo se nacist lockscreen nebo warning obrazek!\n";
        return EXIT_FAILURE;
    }

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

        cv::Mat scene;

        if (faces.empty()) {
            scene = lockscreen.clone();
        }
        else if (faces.size() == 1) {
            frame.copyTo(scene);

            cv::Rect face = faces[0];
            cv::Point2f faceCenter(
                static_cast<float>(face.x + face.width / 2) / frame.cols,
                static_cast<float>(face.y + face.height / 2) / frame.rows
            );

            cv::Mat mask;
            cv::Point2f cupCenter = ImageProcessor::detect_red_object(
                frame, mask,
                cv::Scalar(175, 115, 115), cv::Scalar(180, 255, 255)
            );

            if (cupCenter.x != 0.5f || cupCenter.y != 0.5f)
                CrossDrawer::draw_cross_normalized(scene, cupCenter, 30);

            cv::imshow("Maska hrnku", mask);
        }
        else {
            scene = warning.clone(); // více než jeden obličej → warning
        }

        cv::imshow("Face+Cup Detection", scene);

        if (fps) {
            fps->update();
            if (fps->is_updated())
                std::cout << "FPS: " << fps->get() << std::endl;
        }

        int key = cv::waitKey(30);
        if (key == 27) break;
    }

    cap.release();
    return EXIT_SUCCESS;
}

