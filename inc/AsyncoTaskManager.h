#pragma once
#include "Asynco.h"

#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

class AsyncoTask;

/******************************************************
 *  Async Task Manager
 *  Schedules tasks and assigns them to worker threads.
 *
 * ****************************************************/

class AsyncoTaskManager
{
    ASYNCO_DEFINE_SINGLETON(AsyncoTaskManager)

    ASYNCO_EXPORT void Start(uint32 maxThreads = 1);
    ASYNCO_EXPORT void AddTask(AsyncoTask* task);

private:
    AsyncoTaskManager();

    void RunInTheThread();

    // Mutex to Access Task Queue
    std::mutex                  m_queueLock;

    std::queue<AsyncoTask*>     m_taskQueue;
    std::vector<std::thread>    m_threadPool;
};

