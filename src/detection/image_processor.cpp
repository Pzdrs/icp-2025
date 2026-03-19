#include "detection/image_processor.hpp"
#include "detection/cross_drawer.hpp"

#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

RedRecognizer::RedRecognizer() = default;

cv::Point2f RedRecognizer::find_red(cv::Mat &frame)
{
    if (frame.empty())
    {
        return cv::Point2f(-1.0f, -1.0f);
    }

    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsv, cv::Scalar(175, 115, 115), cv::Scalar(180, 255, 255), mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty())
    {
        return cv::Point2f(-1.0f, -1.0f);
    }

    const double min_area = 0.01 * static_cast<double>(frame.cols * frame.rows);
    double best_area = 0.0;
    cv::Point2f best_center(-1.0f, -1.0f);

    for (const auto &contour : contours)
    {
        const double area = cv::contourArea(contour);
        if (area < min_area || area < best_area)
        {
            continue;
        }

        cv::Moments moments = cv::moments(contour);
        if (moments.m00 == 0.0)
        {
            continue;
        }

        const float cx = static_cast<float>(moments.m10 / moments.m00);
        const float cy = static_cast<float>(moments.m01 / moments.m00);
        best_area = area;
        best_center = cv::Point2f(cx / static_cast<float>(frame.cols), cy / static_cast<float>(frame.rows));
    }

    return best_center;
}

int RedRecognizer::run_static(std::string path)
{
    cv::Mat frame = cv::imread(path);
    if (frame.empty())
    {
        std::cerr << "Failed to open image: " << path << std::endl;
        return EXIT_FAILURE;
    }

    const cv::Point2f center = find_red(frame);
    if (center.x >= 0.0f && center.y >= 0.0f)
    {
        CrossDrawer::draw_cross_normalized(frame, center, 30);
    }

    cv::imshow("Red Detection", frame);
    cv::waitKey(0);
    cv::destroyWindow("Red Detection");
    return EXIT_SUCCESS;
}

int RedRecognizer::run_video(std::string path)
{
    capture_device.open(path);
    if (!capture_device.isOpened())
    {
        std::cerr << "Failed to open video: " << path << std::endl;
        return EXIT_FAILURE;
    }

    cv::Mat frame;
    while (capture_device.read(frame))
    {
        const cv::Point2f center = find_red(frame);
        if (center.x >= 0.0f && center.y >= 0.0f)
        {
            CrossDrawer::draw_cross_normalized(frame, center, 30);
        }

        cv::imshow("Red Detection", frame);
        const int key = cv::waitKey(1);
        if (key == 27 || key == 'q')
        {
            break;
        }
    }

    capture_device.release();
    cv::destroyWindow("Red Detection");
    return EXIT_SUCCESS;
}
