#include "pch.hpp"
#include "thread_pool.hpp"

ThreadPool::ThreadPool(uint32_t threadCount)
{
    for (uint32_t i = 0; i < threadCount; i++)
        m_Workers.emplace_back(&ThreadPool::WorkerLoop, this);
}

ThreadPool::~ThreadPool()
{
    m_Running = false;
    m_CV.notify_all();

    for (auto& t : m_Workers)
        t.join();
}

void ThreadPool::Enqueue(std::function<void()> job)
{
    {
        std::unique_lock lock(m_Mutex);
        m_Jobs.push(std::move(job));
    }
    m_CV.notify_one();
}

void ThreadPool::WorkerLoop()
{
    while (m_Running)
    {
        std::function<void()> job;

        {
            std::unique_lock lock(m_Mutex);

            m_CV.wait(lock, [&]() {
                return !m_Jobs.empty() || !m_Running;
            });

            if (!m_Running)
                return;

            job = std::move(m_Jobs.front());
            m_Jobs.pop();
        }

        job();
    }
}