#pragma once
#include "Asynco.h"

#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

class AsyncoTask;

struct AsyncoTaskBundle
{
    AsyncoTaskBundle()
        : m_handle(nullptr)
        , m_task(nullptr)
        , m_completionCallback(nullptr)
    {

    }

    AsyncoTaskHandle*       m_handle;
    AsyncoTask*             m_task;
    OnAsyncoTaskCompleted*  m_completionCallback;
};

/********************************************************
 *  AsyncoTaskManager
 *  Schedules tasks and assigns them to worker threads.
 * ******************************************************/
class AsyncoTaskManager
{
    ASYNCO_DEFINE_SINGLETON(AsyncoTaskManager)

    ASYNCO_EXPORT void                  Start(uint32 maxThreads = 1, uint32 maxTasksPerThread = 8);
    ASYNCO_EXPORT AsyncoTaskHandle&     AddTask(AsyncoTask* task, OnAsyncoTaskCompleted* callback);

    ASYNCO_EXPORT void                  Update();

private:
    AsyncoTaskManager();

    void                            RunInTheThread();

    // Mutex to Access Tasks
    std::mutex                      m_pendingTasksLock;
    std::mutex                      m_activeTasksLock;

    std::queue<AsyncoTaskBundle*>   m_pendingTasks;
    std::vector<AsyncoTaskBundle*>  m_activeTasks;

    // Active Worker Threads
    std::vector<std::thread>        m_threadPool;

    uint32                          m_maxTasksPerThread;
};
