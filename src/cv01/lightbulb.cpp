#include <lightbulb.hpp>

#define GRAYSCALE_THRESHOLD 240
#define CROSS_SIZE 25

void draw_cross(cv::Mat &img, int x, int y, int size)
{
    cv::Point p1(x - size / 2, y);
    cv::Point p2(x + size / 2, y);
    cv::Point p3(x, y - size / 2);
    cv::Point p4(x, y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}

void draw_cross_normalized(cv::Mat &img, cv::Point2f center_normalized, int size)
{
    center_normalized.x = std::clamp(center_normalized.x, 0.0f, 1.0f);
    center_normalized.y = std::clamp(center_normalized.y, 0.0f, 1.0f);
    size = std::clamp(size, 1, std::min(img.cols, img.rows));

    cv::Point2f center_absolute(center_normalized.x * img.cols, center_normalized.y * img.rows);

    cv::Point2f p1(center_absolute.x - size / 2, center_absolute.y);
    cv::Point2f p2(center_absolute.x + size / 2, center_absolute.y);
    cv::Point2f p3(center_absolute.x, center_absolute.y - size / 2);
    cv::Point2f p4(center_absolute.x, center_absolute.y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}

void doLightbulb()
{
    // načtení obrázku
    cv::Mat frame = cv::imread("./resources/lightbulb.jpg");

    if (frame.empty())
        throw std::runtime_error("Empty file? Wrong path?");

    auto start = std::chrono::steady_clock::now();

    cv::Mat frame2;
    frame.copyTo(frame2);

    cv::Point2f center(0, 0);
    int count_white = 0;

    // jednoduchý převod do grayscale a threshold
    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);

            // konverze do Y (luminance) podle BT.601
            unsigned char Y = static_cast<unsigned char>(
                0.114 * pixel[0] + // Blue
                0.587 * pixel[1] + // Green
                0.299 * pixel[2]); // Red

            if (Y < GRAYSCALE_THRESHOLD)
            {
                frame2.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
            else
            {
                frame2.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
                center.x += x;
                center.y += y;
                count_white++;
            }
        }
    }

    if (count_white > 0)
    {
        center.x /= count_white;
        center.y /= count_white;
    }
    else
    {
        center = cv::Point2f(frame.cols / 2, frame.rows / 2); // fallback
    }

    cv::Point2f center_normalized(center.x / frame.cols, center.y / frame.rows);

    std::cout << "Center absolute: " << center << '\n';
    std::cout << "Center normalized: " << center_normalized << '\n';

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " sec" << std::endl;

    // vykresli kříž
    draw_cross(frame, static_cast<int>(center.x), static_cast<int>(center.y), CROSS_SIZE);
    draw_cross_normalized(frame2, center_normalized, CROSS_SIZE);

    cv::imshow("original", frame);
    cv::imshow("thresholded", frame2);
}