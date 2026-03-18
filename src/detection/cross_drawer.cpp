#include "detection/cross_drawer.hpp"

#include <algorithm>
#include <cmath>

#include <opencv2/imgproc.hpp>

namespace detection
{

    void CrossDrawer::DrawCrossNormalized(cv::Mat &image, const cv::Point2f &normalized,
                                          int size, const cv::Scalar &color, int thickness)
    {
        if (image.empty())
        {
            return;
        }

        if (normalized.x < 0.0f || normalized.x > 1.0f || normalized.y < 0.0f || normalized.y > 1.0f)
        {
            return;
        }

        const int width = image.cols;
        const int height = image.rows;
        const int cx = static_cast<int>(std::round(normalized.x * static_cast<float>(width - 1)));
        const int cy = static_cast<int>(std::round(normalized.y * static_cast<float>(height - 1)));

        const int half = size / 2;

        cv::Point horizontalStart(std::max(0, cx - half), cy);
        cv::Point horizontalEnd(std::min(width - 1, cx + half), cy);
        cv::Point verticalStart(cx, std::max(0, cy - half));
        cv::Point verticalEnd(cx, std::min(height - 1, cy + half));

        cv::line(image, horizontalStart, horizontalEnd, color, thickness);
        cv::line(image, verticalStart, verticalEnd, color, thickness);
    }

} // namespace detection
