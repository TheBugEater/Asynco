#pragma once
#include "Asynco.h"

#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

class AsyncoTask;

/********************************************************
 *  AsyncoTaskHandle
 *  Every Task gets it's own handle
 * ******************************************************/
class AsyncoTaskHandle
{
    // Only AsyncoTaskManager should be able to create handles and change status
    friend class    AsyncoTaskManager;

public:
    uint32                  GetHandleId() const { return m_handleId; }

private:
    AsyncoTaskHandle(uint32 handleId);

    uint32                  m_handleId;
};

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
    friend class AsyncoWorkerThread;

    AsyncoTaskManager();

    // Internal Functions for Threads
    void                            RunInTheThread();
    void                            AddCompletedTask(AsyncoTaskBundle* bundle);

    // Mutex to Access Tasks
    std::mutex                      m_pendingTasksLock;
    std::mutex                      m_completedTasksLock;

    std::queue<AsyncoTaskBundle*>   m_pendingTasks;
    std::vector<AsyncoTaskBundle*>  m_completedTasks;;

    // Active Worker Threads
    std::vector<std::thread>        m_threadPool;

    uint32                          m_maxTasksPerThread;

    uint32                          m_taskHandleNum;
};
