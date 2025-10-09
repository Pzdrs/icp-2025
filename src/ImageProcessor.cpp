#include "ImageProcessor.h"
#include "fps_meter.h"

// ---------- find_object_luma ----------
cv::Point2f ImageProcessor::find_object_luma(const cv::Mat& input, cv::Mat& output, int threshold) {
    input.copyTo(output);

    double sumX = 0, sumY = 0;
    int count_white = 0;

    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            cv::Vec3b pixel = input.at<cv::Vec3b>(y, x);
            unsigned char R = pixel[2];
            unsigned char G = pixel[1];
            unsigned char B = pixel[0];

            unsigned char Y = static_cast<unsigned char>(0.299 * R + 0.587 * G + 0.114 * B);

            if (Y >= threshold) {
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                sumX += x;
                sumY += y;
                count_white++;
            }
            else {
                output.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }

    cv::Point2f center_normalized(0.5f, 0.5f);
    if (count_white > 0) {
        center_normalized.x = static_cast<float>(sumX / count_white / input.cols);
        center_normalized.y = static_cast<float>(sumY / count_white / input.rows);
    }
    return center_normalized;
}

// ---------- draw_cross ----------
void ImageProcessor::draw_cross(cv::Mat& img, int x, int y, int size) {
    cv::line(img, cv::Point(x - size / 2, y), cv::Point(x + size / 2, y), CV_RGB(255, 0, 0), 3);
    cv::line(img, cv::Point(x, y - size / 2), cv::Point(x, y + size / 2), CV_RGB(255, 0, 0), 3);
}

// ---------- draw_cross_normalized ----------
void ImageProcessor::draw_cross_normalized(cv::Mat& img, cv::Point2f center_normalized, int size) {
    center_normalized.x = std::clamp(center_normalized.x, 0.0f, 1.0f);
    center_normalized.y = std::clamp(center_normalized.y, 0.0f, 1.0f);
    size = std::clamp(size, 1, std::min(img.cols, img.rows));

    cv::Point2f center_absolute(center_normalized.x * img.cols, center_normalized.y * img.rows);
    draw_cross(img, static_cast<int>(center_absolute.x), static_cast<int>(center_absolute.y), size);
}

// ---------- detect_red_object ----------
cv::Point2f ImageProcessor::detect_red_object(const cv::Mat& img, cv::Mat& mask,
    cv::Scalar lower, cv::Scalar upper,
    bool useMorph) {
    cv::Mat hsv, mask1, mask2;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, lower, upper, mask);

    if (useMorph) {
        cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, element);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element);
    }

    // centroid
    std::vector<cv::Point> points;
    cv::findNonZero(mask, points);
    cv::Point2f centroid_normalized(0.5f, 0.5f);
    if (!points.empty()) {
        cv::Point2f sum(0.0f, 0.0f);
        for (auto& p : points) sum += cv::Point2f(p.x, p.y);
        centroid_normalized = cv::Point2f(sum.x / points.size() / img.cols,
            sum.y / points.size() / img.rows);
    }

    // vykreslení køíže
    draw_cross_normalized(mask, centroid_normalized, 25);

    return centroid_normalized;
}

// ---------- detect_red_object_video ----------
void ImageProcessor::detect_red_object_video(const std::string& videoPath,
    cv::Scalar lower, cv::Scalar upper,
    bool useMorph,
    FPSMeter* fps) {

    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Nepodarilo se otevrit video!\n";
        return;
    }

    cv::Mat frame, mask;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::Point2f centroid = detect_red_object(frame, mask, lower, upper, useMorph);

        cv::imshow("Grabbed", frame);
        cv::imshow("Maska", mask);

        if (fps) {
            fps->update();
            if (fps->is_updated())
                std::cout << "FPS: " << fps->get() << std::endl;
        }

        int key = cv::waitKey(30);
        if (key == 27) break;
    }

    cap.release();
}



// ---------- detect_face ----------
cv::Point2f ImageProcessor::detect_face(const cv::Mat& frame, cv::CascadeClassifier& face_cascade) {
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray); // zlepší detekci

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

    cv::Point2f center(0.5f, 0.5f); // default støed obrazu

    if (!faces.empty()) {
        cv::Rect face = faces[0];
        center.x = static_cast<float>(face.x + face.width / 2) / frame.cols;
        center.y = static_cast<float>(face.y + face.height / 2) / frame.rows;
    }

    return center;
}
