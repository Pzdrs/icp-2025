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

    cv::Mat mask1, mask2;
    cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsv, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), mask2);

    cv::Mat mask = mask1 | mask2;

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty())
    {
        return cv::Point2f(-1.0f, -1.0f);
    }

    size_t largest_contour_index = 0;
    double largest_area = 0.0;
    for (size_t i = 0; i < contours.size(); ++i)
    {
        double area = cv::contourArea(contours[i]);
        if (area > largest_area)
        {
            largest_area = area;
            largest_contour_index = i;
        }
    }

    cv::Moments moments = cv::moments(contours[largest_contour_index]);
    if (moments.m00 == 0)
    {
        return cv::Point2f(-1.0f, -1.0f);
    }

    return cv::Point2f(static_cast<float>(moments.m10 / moments.m00) / frame.cols,
                       static_cast<float>(moments.m01 / moments.m00) / frame.rows);
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
