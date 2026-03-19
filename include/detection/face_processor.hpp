#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
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
    int start_background_detection(void);
    int stop_background_detection(void);
    void capture_loop(void);
    void tracker_loop(void);

    cv::CascadeClassifier classifier;
    std::unique_ptr<cv::VideoCapture> capture_device;
    std::string cascade_path;
    int camera_index = 0;
    bool show_fps = true;

    std::atomic<bool> terminate_requested{false};
    std::atomic<float> result_x{-1.0f};
    std::atomic<float> result_y{-1.0f};
    std::atomic<int> face_count{0};

    std::thread capture_thread;
    std::thread tracker_thread;

    cv::Mat shared_frame;
    std::mutex frame_mutex;
    std::condition_variable frame_ready_cv;
    std::atomic<bool> frame_ready{false};
};
