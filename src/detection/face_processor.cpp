#include "detection/face_processor.hpp"

#include <iostream>

cv::Mat make_fallback_image(const cv::Scalar &color, const std::string &text)
{
    cv::Mat image(720, 1280, CV_8UC3, color);
    if (!text.empty())
    {
        cv::putText(image, text, cv::Point(40, image.rows / 2), cv::FONT_HERSHEY_SIMPLEX,
                    1.2, cv::Scalar(255, 255, 255), 2);
    }
    return image;
}

FaceRecognizer::FaceRecognizer(std::string cascade_path, int camera_index, bool show_fps)
    : cascade_path(std::move(cascade_path)), camera_index(camera_index), show_fps(show_fps)
{
}

bool FaceRecognizer::init(void)
{
    if (!classifier.load(cascade_path))
    {
        return false;
    }

    capture_device.open(camera_index);
    return capture_device.isOpened();
}

FaceRecognizer::~FaceRecognizer()
{
    if (capture_device.isOpened())
    {
        capture_device.release();
    }
}

std::vector<cv::Point2f> FaceRecognizer::find_face(cv::Mat &frame)
{
    std::vector<cv::Point2f> result;
    if (frame.empty())
    {
        return result;
    }

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    classifier.detectMultiScale(gray, faces);

    for (const auto &face : faces)
    {
        const cv::Point2f center(face.x + face.width / 2.0f, face.y + face.height / 2.0f);
        result.push_back(cv::Point2f(center.x / frame.cols, center.y / frame.rows));
    }

    return result;
}

int FaceRecognizer::run(void)
{
    if (!capture_device.isOpened() && !init())
    {
        std::cerr << "Failed to initialize face recognizer." << std::endl;
        return EXIT_FAILURE;
    }

    RedRecognizer red_recognizer;
    fps_meter fps;
    const cv::Mat lockscreen = make_fallback_image(cv::Scalar(40, 40, 40), "Waiting for face...");
    const cv::Mat warning = make_fallback_image(cv::Scalar(10, 10, 160), "Multiple faces detected!");

    cv::Mat frame;
    while (capture_device.read(frame))
    {
        cv::Mat scene;
        std::vector<cv::Point2f> faces = find_face(frame);

        if (faces.empty())
        {
            scene = lockscreen.clone();
        }
        else if (faces.size() == 1)
        {
            scene = frame.clone();
            const cv::Point2f red_center = red_recognizer.find_red(scene);
            if (red_center.x >= 0.0f && red_center.y >= 0.0f)
            {
                CrossDrawer::draw_cross_normalized(scene, red_center, 30);
            }
        }
        else
        {
            scene = warning.clone();
        }

        if (scene.empty())
        {
            scene = make_fallback_image(cv::Scalar(0, 0, 0), "No frame");
        }

        cv::imshow("Face+Cup Detection", scene);

        if (show_fps)
        {
            fps.update();
            if (fps.is_updated())
            {
                std::cout << "FPS: " << fps.get() << std::endl;
            }
        }

        const int key = cv::waitKey(1);
        if (key == 27 || key == 'q')
        {
            break;
        }
    }

    cv::destroyWindow("Face+Cup Detection");
    return EXIT_SUCCESS;
}
