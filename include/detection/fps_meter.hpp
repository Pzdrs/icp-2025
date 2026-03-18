#pragma once

#include <chrono>

namespace detection
{

    class FPSMeter
    {
    public:
        FPSMeter();

        void Reset();
        void Update();
        bool IsUpdated() const { return m_Updated; }
        double Get() const { return m_LastFPS; }

    private:
        using Clock = std::chrono::steady_clock;

        Clock::time_point m_LastTick;
        Clock::time_point m_LastReport;
        double m_LastFPS = 0.0;
        bool m_Updated = false;
        std::uint32_t m_FrameCounter = 0;
    };

}
