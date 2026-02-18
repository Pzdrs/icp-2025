#pragma once
#include <functional>
#include <thread>

class ThreadPool
{
public:
    ThreadPool(uint32_t threadCount);
    ~ThreadPool();

    void Enqueue(std::function<void()> job);

private:
    void WorkerLoop();

private:
    std::vector<std::thread> m_Workers;
    std::queue<std::function<void()>> m_Jobs;

    std::mutex m_Mutex;
    std::condition_variable m_CV;
    std::atomic<bool> m_Running = true;
};