#include "CrossDrawer.h"

void CrossDrawer::draw_cross(cv::Mat& img, int x, int y, int size) {
    cv::line(img, cv::Point(x - size / 2, y), cv::Point(x + size / 2, y), CV_RGB(255, 0, 0), 3);
    cv::line(img, cv::Point(x, y - size / 2), cv::Point(x, y + size / 2), CV_RGB(255, 0, 0), 3);
}

void CrossDrawer::draw_cross_normalized(cv::Mat& img, cv::Point2f center_normalized, int size) {
    center_normalized.x = std::clamp(center_normalized.x, 0.0f, 1.0f);
    center_normalized.y = std::clamp(center_normalized.y, 0.0f, 1.0f);
    size = std::clamp(size, 1, std::min(img.cols, img.rows));

    cv::Point2f center_absolute(center_normalized.x * img.cols, center_normalized.y * img.rows);
    draw_cross(img, static_cast<int>(center_absolute.x), static_cast<int>(center_absolute.y), size);
}
