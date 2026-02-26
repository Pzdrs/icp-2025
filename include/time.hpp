#pragma once
#include <cstdint>
#include <chrono>

class PlatformClock
{
public:
    // current time in ns
    static uint64_t Now()
    {
        using clock = std::chrono::steady_clock;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
                   clock::now().time_since_epoch())
            .count();
    }
};

class Time
{
public:
    static void Init();
    static void Update();

    static float DeltaTime();         // scaled seconds
    static float UnscaledDeltaTime(); // raw seconds
    static double TotalTime();        // double for long sessions

    static void SetTimeScale(float scale);
    static float GetTimeScale() { return s_TimeScale; }
    static void Pause(bool pause);
    static bool IsPaused() { return s_Paused; }

    static bool ShouldRunFixedStep();
    static void ConsumeFixedStep();
    static float FixedDeltaTime();

private:
    inline static uint64_t s_LastTick = 0;
    inline static uint64_t s_TotalTicks = 0;

    inline static uint64_t s_Accumulator = 0;

    inline static float s_DeltaTime = 0.0f;
    inline static float s_UnscaledDeltaTime = 0.0f;
    inline static float s_TimeScale = 1.0f;
    inline static bool s_Paused = false;

    static constexpr double NS_TO_SEC = 1.0 / 1'000'000'000.0;
    static constexpr uint64_t FIXED_TICK_NS = 16'666'667; // 1/60th of a second in nanoseconds
    static constexpr uint64_t MAX_ACCUMULATED = FIXED_TICK_NS * 5;
};