#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "detection/cross_drawer.hpp"
#include "detection/fps_meter.hpp"
#include "detection/image_processor.hpp"

class FaceRecognizer
{
public:
    FaceRecognizer(std::string cascade_path = {}, int camera_index = 0, bool show_fps = true);
    bool init(void);
    int run(void);
    ~FaceRecognizer();
    std::vector<cv::Point2f> find_face(cv::Mat &frame);

private:
    cv::CascadeClassifier classifier;
    cv::VideoCapture capture_device;
    std::string cascade_path;
    int camera_index = 0;
    bool show_fps = true;
};
