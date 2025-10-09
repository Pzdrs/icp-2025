#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

class FPSMeter;

class ImageProcessor {
public:
    ImageProcessor() = default;

    // støed podle Y kanálu (zarovka)
    static cv::Point2f find_object_luma(const cv::Mat& input, cv::Mat& output, int threshold = 230);

    // kreslení køíže
    static void draw_cross(cv::Mat& img, int x, int y, int size);
    static void draw_cross_normalized(cv::Mat& img, cv::Point2f center_normalized, int size);

    // detekce cerveneho objektu
    static cv::Point2f detect_red_object(const cv::Mat& img, cv::Mat& mask,
        cv::Scalar lower, cv::Scalar upper,
        bool useMorph = true);

    static void detect_red_object_video(const std::string& videoPath,
        cv::Scalar lower, cv::Scalar upper,
        bool useMorph = true,
        FPSMeter* fps = nullptr);


    // detekce oblièeje z obrazu
    static cv::Point2f detect_face(const cv::Mat& frame, cv::CascadeClassifier& face_cascade);
};
