#pragma once
#include "Asynco.h"

#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

class AsyncoTask;

enum class ETaskStatus
{
    Idle,
    InProgress,
    Completed
};

class AsyncoTaskResult
{
    template<typename T>
    T* GetResult()
    {
        return static_cast<T*>(this);
    }
};

// Function Pointer to notify Task Completion
typedef void(OnAsyncoTaskCompleted)(AsyncoTaskResult*);

/********************************************************
 *  AsyncoTaskHandle
 *  Every Task gets it's own handle to query the state
 * ******************************************************/
class AsyncoTaskHandle
{
    // Only AsyncoTaskManager should be able to create handles and change status
    friend class    AsyncoTaskManager;

public:
    uint32          GetHandleId() const { return m_handleId; }
    ETaskStatus     GetStatus() const { return m_taskStatus; }

private:
    AsyncoTaskHandle(uint32 handleId);

    void                    SetStatus(ETaskStatus status) { m_taskStatus = status; }

    uint32                  m_handleId;
    ETaskStatus             m_taskStatus;
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

    ASYNCO_EXPORT void                  Start(uint32 maxThreads = 1);
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
};
