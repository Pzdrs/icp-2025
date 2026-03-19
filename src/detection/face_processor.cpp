#include "detection/face_processor.hpp"

#include <chrono>
#include <iostream>

cv::Mat make_fallback_image(const cv::Scalar &color, const std::string &text)
{
    cv::Mat image(720, 1280, CV_8UC3, color);
    if (!text.empty())
    {
        cv::putText(image, text, cv::Point(40, image.rows / 2), cv::FONT_HERSHEY_SIMPLEX,
                    1.2, cv::Scalar(255, 255, 255), 2);
    }
    return image;
}

FaceRecognizer::FaceRecognizer(std::string cascade_path, int camera_index, bool show_fps)
    : cascade_path(std::move(cascade_path)), camera_index(camera_index), show_fps(show_fps)
{
}

bool FaceRecognizer::init(void)
{
    if (!classifier.load(cascade_path))
    {
        return false;
    }

    return true;
}

FaceRecognizer::~FaceRecognizer()
{
    stop_background_detection();
}

std::vector<cv::Point2f> FaceRecognizer::find_face(cv::Mat &frame)
{
    std::vector<cv::Point2f> result;
    if (frame.empty())
    {
        return result;
    }

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> faces;
    classifier.detectMultiScale(gray, faces);

    for (const auto &face : faces)
    {
        const cv::Point2f center(face.x + face.width / 2.0f, face.y + face.height / 2.0f);
        result.push_back(cv::Point2f(center.x / frame.cols, center.y / frame.rows));
    }

    return result;
}

int FaceRecognizer::run(void)
{
    if (start_background_detection() != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    fps_meter fps;
    const cv::Mat lockscreen = make_fallback_image(cv::Scalar(40, 40, 40), "Waiting for face...");
    const cv::Mat warning = make_fallback_image(cv::Scalar(10, 10, 160), "Multiple faces detected!");

    cv::Mat latest_frame;
    while (!terminate_requested.load())
    {
        {
            std::scoped_lock lock(frame_mutex);
            if (!shared_frame.empty())
            {
                latest_frame = shared_frame.clone();
            }
        }

        cv::Mat scene;
        const int count = face_count.load();
        const float x = result_x.load();
        const float y = result_y.load();

        if (count <= 0)
        {
            scene = lockscreen.clone();
        }
        else if (count == 1)
        {
            if (!latest_frame.empty())
            {
                scene = latest_frame;
                if (x >= 0.0f && y >= 0.0f)
                {
                    CrossDrawer::draw_cross_normalized(scene, cv::Point2f(x, y), 30);
                }
            }
            else
            {
                scene = lockscreen.clone();
            }
        }
        else
        {
            scene = warning.clone();
        }

        if (scene.empty())
        {
            scene = make_fallback_image(cv::Scalar(0, 0, 0), "No frame");
        }

        cv::imshow("Face+Cup Detection", scene);

        if (show_fps)
        {
            fps.update();
            if (fps.is_updated())
            {
                std::cout << "FPS: " << fps.get() << std::endl;
            }
        }

        const int key = cv::waitKey(1);
        if (key == 27 || key == 'q')
        {
            terminate_requested.store(true);
        }
    }

    cv::destroyWindow("Face+Cup Detection");
    return stop_background_detection();
}

int FaceRecognizer::start_background_detection(void)
{
    if (capture_device && capture_device->isOpened())
    {
        std::cerr << "Background detection is already running." << std::endl;
        return EXIT_FAILURE;
    }

    capture_device = std::make_unique<cv::VideoCapture>(camera_index);
    if (!capture_device->isOpened())
    {
        std::cerr << "Failed to open camera index " << camera_index << std::endl;
        capture_device.reset();
        return EXIT_FAILURE;
    }

    terminate_requested.store(false);
    result_x.store(-1.0f);
    result_y.store(-1.0f);
    face_count.store(0);
    frame_ready.store(false);

    capture_thread = std::thread(&FaceRecognizer::capture_loop, this);
    tracker_thread = std::thread(&FaceRecognizer::tracker_loop, this);

    return EXIT_SUCCESS;
}

int FaceRecognizer::stop_background_detection(void)
{
    terminate_requested.store(true);
    frame_ready_cv.notify_all();

    if (capture_thread.joinable())
    {
        capture_thread.join();
    }

    if (tracker_thread.joinable())
    {
        tracker_thread.join();
    }

    if (capture_device)
    {
        capture_device->release();
        capture_device.reset();
    }

    return EXIT_SUCCESS;
}

void FaceRecognizer::capture_loop(void)
{
    cv::Mat local;
    while (!terminate_requested.load())
    {
        if (!capture_device || !capture_device->isOpened())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        (*capture_device) >> local;
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
        frame_ready_cv.notify_one();
    }
}

void FaceRecognizer::tracker_loop(void)
{
    RedRecognizer red_recognizer;
    cv::Mat frame;
    while (!terminate_requested.load())
    {
        {
            std::unique_lock lock(frame_mutex);
            frame_ready_cv.wait(lock, [this] {
                return terminate_requested.load() || frame_ready.load();
            });
            if (terminate_requested.load())
            {
                break;
            }
            if (shared_frame.empty())
            {
                frame_ready.store(false);
                continue;
            }
            frame = shared_frame.clone();
            frame_ready.store(false);
        }

        const std::vector<cv::Point2f> faces = find_face(frame);
        face_count.store(static_cast<int>(faces.size()));

        if (faces.size() == 1)
        {
            const cv::Point2f red_center = red_recognizer.find_red(frame);
            result_x.store(red_center.x);
            result_y.store(red_center.y);
        }
        else
        {
            result_x.store(-1.0f);
            result_y.store(-1.0f);
        }
    }
}
