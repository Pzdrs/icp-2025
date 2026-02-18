#include "pch.hpp"
#include "job_system.hpp"

void JobSystem::Init(uint32_t workers)
{
    s_ThreadPool = CreateScope<ThreadPool>(workers);
}

void JobSystem::Shutdown()
{
    s_ThreadPool.reset();
}

void JobSystem::Dispatch(std::function<void()> job)
{
    s_ThreadPool->Enqueue(std::move(job));
}