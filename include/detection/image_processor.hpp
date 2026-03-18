#pragma once

#include <opencv2/core.hpp>

namespace detection
{

    class ImageProcessor
    {
    public:
        // Detects the largest red object in the HSV interval.
        // Returns normalized coordinates (0..1). If nothing is found, (-1, -1) is returned.
        static cv::Point2f DetectRedObject(const cv::Mat &inputImg, cv::Mat &mask,
                                           const cv::Scalar &lowerBound,
                                           const cv::Scalar &upperBound,
                                           float minAreaRatio = 0.01f);
    };

}
