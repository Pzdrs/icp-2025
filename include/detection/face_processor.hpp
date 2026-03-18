#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <opencv2/opencv.hpp>

#include "detection/cross_drawer.hpp"
#include "detection/fps_meter.hpp"
#include "detection/image_processor.hpp"

namespace detection
{

    class FaceProcessor
    {
    public:
        FaceProcessor(const std::string &cascadePath, int cameraIndex = 0);
        ~FaceProcessor();

        int RunFromCamera(FPSMeter *fps = nullptr);
        int RunFromCameraPlus(FPSMeter *fps = nullptr);
        int RunFromCameraPlusFPS(FPSMeter *fps = nullptr);

        int StartBackgroundDetection(FPSMeter *fps = nullptr);
        bool IsFaceDetected() const;
        int StopBackgroundDetection();

        cv::Point2f DetectFace(const cv::Mat &frame);

        void RequestStop() { terminate_requested = true; }

    private:
        void CaptureLoop();
        void TrackerLoop();

        cv::CascadeClassifier m_FaceCascade;
        int m_CameraIndex = 0;

        std::unique_ptr<cv::VideoCapture> m_Capture;
        std::atomic<bool> terminate_requested{false};
        std::atomic<float> result_x{-1.0f};
        std::atomic<float> result_y{-1.0f};
        std::atomic<int> face_count{0};

        std::thread capture_thread;
        std::thread tracker_thread;

        cv::Mat shared_frame;
        std::mutex frame_mutex;
        std::atomic<bool> frame_ready{false};
    };

}
