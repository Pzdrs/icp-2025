#include "detection/face_processor.hpp"

#include <chrono>
#include <iostream>
#include <vector>

namespace detection
{
    namespace
    {

        cv::Mat CreateFallbackImage(const cv::Scalar &color, const std::string &text)
        {
            cv::Mat image(720, 1280, CV_8UC3, color);
            if (!text.empty())
            {
                cv::putText(image, text, cv::Point(40, image.rows / 2), cv::FONT_HERSHEY_SIMPLEX,
                            1.2, cv::Scalar(255, 255, 255), 2);
            }
            return image;
        }
    } // namespace

    FaceProcessor::FaceProcessor(const std::string &cascadePath, int cameraIndex)
        : m_CameraIndex(cameraIndex)
    {
        if (!m_FaceCascade.load(cascadePath))
        {
            throw std::runtime_error("Failed to load Haar cascade: " + cascadePath);
        }
    }

    FaceProcessor::~FaceProcessor()
    {
        StopBackgroundDetection();
    }

    int FaceProcessor::RunFromCamera(FPSMeter *fps)
    {
        return RunFromCameraPlus(fps);
    }

    int FaceProcessor::RunFromCameraPlus(FPSMeter *fps)
    {
        return RunFromCameraPlusFPS(fps);
    }

    int FaceProcessor::RunFromCameraPlusFPS(FPSMeter *fps)
    {
        if (StartBackgroundDetection(fps) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

        const cv::Mat lockscreen = CreateFallbackImage(cv::Scalar(40, 40, 40), "Waiting for face...");
        const cv::Mat warning = CreateFallbackImage(cv::Scalar(10, 10, 160), "Multiple faces detected!");

        cv::Mat latestFrame;
        while (!terminate_requested.load())
        {
            {
                std::scoped_lock lock(frame_mutex);
                if (!shared_frame.empty())
                {
                    latestFrame = shared_frame.clone();
                }
            }

            const int count = face_count.load();
            const float x = result_x.load();
            const float y = result_y.load();

            cv::Mat scene;
            if (count <= 0)
            {
                scene = lockscreen.clone();
            }
            else if (count == 1 && !latestFrame.empty())
            {
                scene = latestFrame;
                if (x >= 0.0f && y >= 0.0f)
                {
                    CrossDrawer::DrawCrossNormalized(scene, cv::Point2f(x, y), 30);
                }
            }
            else
            {
                scene = warning.clone();
            }

            if (scene.empty())
            {
                scene = latestFrame.empty() ? CreateFallbackImage(cv::Scalar(0, 0, 0), "No frame")
                                            : latestFrame;
            }

            cv::imshow("Face+Cup Detection", scene);

            if (fps)
            {
                fps->Update();
                if (fps->IsUpdated())
                {
                    std::cout << "FPS: " << fps->Get() << std::endl;
                }
            }

            const int key = cv::waitKey(1);
            if (key == 27 || key == 'q')
            {
                terminate_requested.store(true);
            }
        }

        cv::destroyWindow("Face+Cup Detection");
        return StopBackgroundDetection();
    }

    int FaceProcessor::StartBackgroundDetection(FPSMeter * /*fps*/)
    {
        if (m_Capture && m_Capture->isOpened())
        {
            std::cerr << "Background detection is already running." << std::endl;
            return EXIT_FAILURE;
        }

        m_Capture = std::make_unique<cv::VideoCapture>(m_CameraIndex);
        if (!m_Capture->isOpened())
        {
            std::cerr << "Failed to open camera index " << m_CameraIndex << std::endl;
            m_Capture.reset();
            return EXIT_FAILURE;
        }

        terminate_requested.store(false);
        result_x.store(-1.0f);
        result_y.store(-1.0f);
        face_count.store(0);
        frame_ready.store(false);

        capture_thread = std::thread(&FaceProcessor::CaptureLoop, this);
        tracker_thread = std::thread(&FaceProcessor::TrackerLoop, this);

        return EXIT_SUCCESS;
    }

    bool FaceProcessor::IsFaceDetected() const
    {
        return face_count.load() == 1;
    }

    int FaceProcessor::StopBackgroundDetection()
    {
        terminate_requested.store(true);

        if (capture_thread.joinable())
        {
            capture_thread.join();
        }

        if (tracker_thread.joinable())
        {
            tracker_thread.join();
        }

        if (m_Capture)
        {
            m_Capture->release();
            m_Capture.reset();
        }

        return EXIT_SUCCESS;
    }

    cv::Point2f FaceProcessor::DetectFace(const cv::Mat &frame)
    {
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        m_FaceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

        cv::Point2f center(-1.0f, -1.0f);
        if (!faces.empty())
        {
            const cv::Rect &face = faces[0];
            center.x = static_cast<float>(face.x + face.width / 2) / frame.cols;
            center.y = static_cast<float>(face.y + face.height / 2) / frame.rows;
        }
        return center;
    }

    void FaceProcessor::CaptureLoop()
    {
        cv::Mat local;
        while (!terminate_requested.load())
        {
            if (!m_Capture || !m_Capture->isOpened())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            (*m_Capture) >> local;
            if (local.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }

            {
                std::scoped_lock lock(frame_mutex);
                shared_frame = local.clone();
                frame_ready.store(true);
            }
        }
    }

    void FaceProcessor::TrackerLoop()
    {
        cv::Mat frame;
        while (!terminate_requested.load())
        {
            if (!frame_ready.load())
            {
                std::this_thread::yield();
                continue;
            }

            {
                std::scoped_lock lock(frame_mutex);
                if (shared_frame.empty())
                {
                    frame_ready.store(false);
                    continue;
                }
                frame = shared_frame.clone();
                frame_ready.store(false);
            }

            cv::Mat gray;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::equalizeHist(gray, gray);

            std::vector<cv::Rect> faces;
            m_FaceCascade.detectMultiScale(gray, faces, 1.2, 3, 0, cv::Size(40, 40));
            face_count.store(static_cast<int>(faces.size()));

            if (faces.size() == 1)
            {
                cv::Mat mask;
                const cv::Point2f cupCenter = ImageProcessor::DetectRedObject(
                    frame, mask, cv::Scalar(175, 115, 115), cv::Scalar(180, 255, 255));
                result_x.store(cupCenter.x);
                result_y.store(cupCenter.y);
            }
            else
            {
                result_x.store(-1.0f);
                result_y.store(-1.0f);
            }
        }
    }

}
