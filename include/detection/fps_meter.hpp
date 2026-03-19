/*
 *  fps_meter.hpp
 *
 *  Author: jiri
 */
#pragma once

#include <chrono>

using namespace std::chrono_literals;

//  A simple class to measure Frames Per Second (FPS) with a configurable update interval.
class fps_meter
{
public:
    // Do not allow type conversion from integers, bool, float etc.
    explicit fps_meter(std::chrono::duration<double> interval = 1.0s) : m_interval(interval) {}

    // Get last FPS value without modification
    double get(void) { return m_fps; }

    // Check for new value
    bool is_updated(void) { return m_updated; }

    // Call once per frame (end of the frame).
    void update(void)
    {
        m_frame_count++;

        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> delta = now - m_last_time;

        if (delta > m_interval)
        {
            m_fps = static_cast<double>(m_frame_count) / delta.count();
            m_frame_count = 0;
            m_last_time = now;
            m_updated = true;
        }
        else
        {
            m_updated = false;
        }
    }

    // Restart frame counting
    void reset(void)
    {
        m_last_time = std::chrono::steady_clock::now();
        m_frame_count = 0;
        m_updated = false;
    }

    // Set different interval for FPS calculation (eg.: increase, if you want to display FPS in window title)
    void set_interval(std::chrono::duration<double> interval)
    {
        m_interval = interval;
    }

private:
    double m_fps{0.0};
    std::chrono::time_point<std::chrono::steady_clock> m_last_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> m_interval = 1.0s;
    size_t m_frame_count{0};
    bool m_updated{false};
};