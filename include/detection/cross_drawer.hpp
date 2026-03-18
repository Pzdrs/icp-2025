#pragma once

#include <opencv2/core.hpp>

namespace detection
{

    class CrossDrawer
    {
    public:
        // Draws a cross at normalized coordinates in the target image.
        static void DrawCrossNormalized(cv::Mat &image, const cv::Point2f &normalized,
                                        int size = 30,
                                        const cv::Scalar &color = cv::Scalar(0, 255, 0),
                                        int thickness = 2);
    };

}
