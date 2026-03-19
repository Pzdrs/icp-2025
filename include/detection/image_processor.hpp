#pragma once

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class RedRecognizer
{
public:
    RedRecognizer();
    int run_static(std::string path);
    int run_video(std::string path);
    cv::Point2f find_red(cv::Mat &frame);

private:
    cv::VideoCapture capture_device;
};
