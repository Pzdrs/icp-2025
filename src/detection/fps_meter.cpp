#include "detection/fps_meter.hpp"

namespace detection
{

    FPSMeter::FPSMeter()
    {
        Reset();
    }

    void FPSMeter::Reset()
    {
        const auto now = Clock::now();
        m_LastTick = now;
        m_LastReport = now;
        m_LastFPS = 0.0;
        m_FrameCounter = 0;
        m_Updated = false;
    }

    void FPSMeter::Update()
    {
        const auto now = Clock::now();
        ++m_FrameCounter;

        const auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - m_LastReport);
        if (elapsed.count() >= 1.0)
        {
            m_LastFPS = static_cast<double>(m_FrameCounter) / elapsed.count();
            m_FrameCounter = 0;
            m_LastReport = now;
            m_Updated = true;
        }
        else
        {
            m_Updated = false;
        }

        m_LastTick = now;
    }

}
