#pragma once
#include "core.hpp"
#include "thread_pool.hpp"

class JobSystem
{
public:
    static void Init(uint32_t workers);
    static void Shutdown();

    static void Dispatch(std::function<void()> job);

private:
    static inline Scope<ThreadPool> s_ThreadPool;
};