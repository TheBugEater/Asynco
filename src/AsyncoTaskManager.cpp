#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"
#include "AsyncoWorkerThread.h"

ASYNCO_IMPLEMENT_SINGLETON(AsyncoTaskManager)

AsyncoTaskHandle::AsyncoTaskHandle(uint32 handleId)
    : m_handleId(handleId)
    , m_taskStatus(ETaskStatus::Idle)
{
}

AsyncoTaskManager::AsyncoTaskManager()
{
}

void AsyncoTaskManager::RunInTheThread()
{
    AsyncoWorkerThread worker;
    while(true)
    {

        // Check if we can Assign New Tasks to this Worker
        auto currentNumTasks = worker.GetCurrentNumTasks();
        if(currentNumTasks < m_maxTasksPerThread)
        {
            uint32 maxPossibleTasks = m_maxTasksPerThread - currentNumTasks;

            std::vector<AsyncoTaskBundle*> pickedTasks;

            {
                std::lock_guard<std::mutex> lock(m_pendingTasksLock);

                while(!m_pendingTasks.empty() && maxPossibleTasks > 0)
                {
                    auto currentJob = m_pendingTasks.front();
                    m_pendingTasks.pop();

                    pickedTasks.push_back(currentJob);

                    --maxPossibleTasks;
                }
            }

            {
                std::lock_guard<std::mutex> lock(m_activeTasksLock);

                for(auto bundle : pickedTasks)
                {
                    m_activeTasks.push_back(bundle);
                    worker.AssignTask(bundle);
                }
            }
        }

        worker.Update(0);
    }
}

void AsyncoTaskManager::Start(uint32 maxThreads, uint32 maxTasksPerThread)
{
    m_maxTasksPerThread = maxTasksPerThread;
    for(uint32 i = 0; i < maxThreads; i++)
    {
        m_threadPool.push_back(std::thread(&AsyncoTaskManager::RunInTheThread, this));
    }
}

AsyncoTaskHandle& AsyncoTaskManager::AddTask(AsyncoTask* task, OnAsyncoTaskCompleted* callback)
{
    AsyncoTaskBundle* bundle = new AsyncoTaskBundle;
    bundle->m_task = task;
    bundle->m_handle = new AsyncoTaskHandle(0);
    bundle->m_completionCallback = callback;

    {
        std::lock_guard<std::mutex> guard(m_pendingTasksLock);
        m_pendingTasks.push(bundle);
    }

    return *bundle->m_handle;
}

void AsyncoTaskManager::Update()
{
    std::vector<AsyncoTaskBundle*> completedTasks;

    {
        std::lock_guard<std::mutex> lock(m_activeTasksLock);

        auto it = m_activeTasks.begin();
        while(it != m_activeTasks.end())
        {
            auto bundle = *it;
            if (bundle->m_handle->GetStatus() == ETaskStatus::Completed)
            {
                it = m_activeTasks.erase(it);
                completedTasks.push_back(bundle);
            }
            else
            {
                ++it;
            }
        }
    }

    for (auto bundle : completedTasks)
    {
        bundle->m_completionCallback(bundle->m_task->GetResult());
    }
}
