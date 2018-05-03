#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"

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
    while(true)
    {
        AsyncoTaskBundle* currentJob = nullptr;

        {
            std::lock_guard<std::mutex> lock(m_pendingTasksLock);

            if(!m_pendingTasks.empty())
            {
                currentJob = m_pendingTasks.front();
                m_pendingTasks.pop();
                std::cout << "Picked a New Job for Thread: " << std::this_thread::get_id() << "\n";
            }
        }

        if(currentJob)
        {
            {
                std::lock_guard<std::mutex> lock(m_activeTasksLock);

                m_activeTasks.push_back(currentJob);
            }

            currentJob->m_handle->SetStatus(ETaskStatus::InProgress);

            currentJob->m_task->DoInBackground();

            currentJob->m_handle->SetStatus(ETaskStatus::Completed);
        }
    }
}

void AsyncoTaskManager::Start(uint32 maxThreads)
{
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
            if (bundle->m_handle->m_taskStatus == ETaskStatus::Completed)
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
        bundle->m_completionCallback(nullptr);
    }
}
