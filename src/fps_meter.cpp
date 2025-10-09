#include "fps_meter.h"

FPSMeter::FPSMeter(std::chrono::duration<double> interval)
    : m_interval(interval),
    m_last_time(std::chrono::steady_clock::now()) {
}

double FPSMeter::get() const {
    return m_fps;
}

bool FPSMeter::is_updated() const {
    return m_updated;
}

void FPSMeter::update() {
    m_frame_count++;

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta = now - m_last_time;

    if (delta > m_interval) {
        m_fps = static_cast<double>(m_frame_count) / delta.count();
        m_frame_count = 0;
        m_last_time = now;
        m_updated = true;
    }
    else {
        m_updated = false;
    }
}

void FPSMeter::reset() {
    m_last_time = std::chrono::steady_clock::now();
    m_frame_count = 0;
    m_fps = 0.0;
    m_updated = false;
}

void FPSMeter::set_interval(std::chrono::duration<double> interval) {
    m_interval = interval;
}
