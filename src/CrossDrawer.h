#pragma once
#include <opencv2/opencv.hpp>
#include <algorithm>

class CrossDrawer {
public:
    static void draw_cross(cv::Mat& img, int x, int y, int size);
    static void draw_cross_normalized(cv::Mat& img, cv::Point2f center_normalized, int size);
};
