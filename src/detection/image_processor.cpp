#include "detection/image_processor.hpp"

#include <opencv2/imgproc.hpp>

namespace detection
{

    cv::Point2f ImageProcessor::DetectRedObject(const cv::Mat &inputImg, cv::Mat &mask,
                                                const cv::Scalar &lowerBound,
                                                const cv::Scalar &upperBound,
                                                float minAreaRatio)
    {
        if (inputImg.empty())
        {
            return cv::Point2f(-1.0f, -1.0f);
        }

        cv::Mat hsv;
        cv::cvtColor(inputImg, hsv, cv::COLOR_BGR2HSV);

        cv::Mat thresholdMask;
        cv::inRange(hsv, lowerBound, upperBound, thresholdMask);

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(thresholdMask, thresholdMask, cv::MORPH_OPEN, kernel);
        cv::morphologyEx(thresholdMask, thresholdMask, cv::MORPH_CLOSE, kernel);

        mask = thresholdMask;

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresholdMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        if (contours.empty())
        {
            return cv::Point2f(-1.0f, -1.0f);
        }

        const double minArea = minAreaRatio * static_cast<double>(inputImg.cols * inputImg.rows);
        double bestArea = 0.0;
        cv::Point2f bestCenter(-1.0f, -1.0f);

        for (const auto &contour : contours)
        {
            const double area = cv::contourArea(contour);
            if (area < minArea || area < bestArea)
            {
                continue;
            }

            cv::Moments m = cv::moments(contour);
            if (m.m00 == 0.0)
            {
                continue;
            }

            const float cx = static_cast<float>(m.m10 / m.m00);
            const float cy = static_cast<float>(m.m01 / m.m00);

            bestArea = area;
            bestCenter = cv::Point2f(cx / static_cast<float>(inputImg.cols),
                                     cy / static_cast<float>(inputImg.rows));
        }

        if (bestArea <= 0.0)
        {
            return cv::Point2f(-1.0f, -1.0f);
        }

        return bestCenter;
    }

}
