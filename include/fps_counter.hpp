#pragma once
#include "time.hpp"
#include <algorithm>

class FPSCounter
{
public:
    FPSCounter(float updateInterval = 0.5f)
        : m_UpdateInterval(updateInterval) {}

    // Call once per frame
    void Update()
    {
        float dt = Time::UnscaledDeltaTime();

        m_Accumulator += dt;
        m_FrameCount++;

        if (m_Accumulator >= m_UpdateInterval)
        {
            m_FPS = static_cast<float>(m_FrameCount) / m_Accumulator;
            m_FrameCount = 0;
            m_Accumulator = 0.0f;
        }

        // Smooth frame time display
        m_FrameTime = m_FrameTime * 0.9f + dt * 0.1f;
    }

    float GetFPS() const { return m_FPS; }
    float GetFrameTimeMS() const { return m_FrameTime * 1000.0f; }

private:
    float m_UpdateInterval = 0.0f; // seconds between FPS updates
    float m_Accumulator = 0.0f;    // accumulated time
    int m_FrameCount = 0;          // frames counted in interval
    float m_FPS = 0.0f;            // smoothed FPS
    float m_FrameTime = 0.0f;      // last frame time in seconds
};