#include "pch.hpp"
#include "time.hpp"

void Time::Init()
{
    s_LastTick = PlatformClock::Now();
}

void Time::Update()
{
    uint64_t current = PlatformClock::Now();
    uint64_t deltaTicks = current - s_LastTick;
    s_LastTick = current;

    s_UnscaledDeltaTime = static_cast<float>(deltaTicks * NS_TO_SEC);

    if (s_Paused)
        s_DeltaTime = 0.0f;
    else
        s_DeltaTime = s_UnscaledDeltaTime * s_TimeScale;

    s_TotalTicks += deltaTicks;

    if (!s_Paused)
    {
        s_Accumulator += static_cast<uint64_t>(deltaTicks * s_TimeScale);
        if (s_Accumulator > MAX_ACCUMULATED)
            s_Accumulator = MAX_ACCUMULATED;
    }
}

float Time::DeltaTime()
{
    return s_DeltaTime;
}

float Time::UnscaledDeltaTime()
{
    return s_UnscaledDeltaTime;
}

double Time::TotalTime()
{
    return static_cast<double>(s_TotalTicks) * NS_TO_SEC;
}

void Time::SetTimeScale(float scale)
{
    s_TimeScale = scale;
}

void Time::Pause(bool pause)
{
    s_Paused = pause;
}

bool Time::ShouldRunFixedStep()
{
    return s_Accumulator >= FIXED_TICK_NS;
}

void Time::ConsumeFixedStep()
{
    s_Accumulator -= FIXED_TICK_NS;
}

float Time::FixedDeltaTime()
{
    return static_cast<float>(FIXED_TICK_NS * NS_TO_SEC);
}