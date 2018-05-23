#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"
#include "AsyncoWorkerThread.h"

#include <ctime>

ASYNCO_IMPLEMENT_SINGLETON(AsyncoTaskManager)

AsyncoTaskHandle::AsyncoTaskHandle(uint32 handleId)
    : m_handleId(handleId)
{
}

AsyncoTaskManager::AsyncoTaskManager()
{
    // Generate a Random Number to Start with 
    std::srand((unsigned int)std::time(nullptr)); 
    m_taskHandleNum = std::rand();
}

void AsyncoTaskManager::RunInTheThread()
{
    AsyncoWorkerThread worker;

    std::clock_t lastFrameTime = std::clock();

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

            for(auto bundle : pickedTasks)
            {
                worker.AssignTask(bundle);
            }
        }


        // Calculate Delta
        std::clock_t currentFrameTime = std::clock();
        float delta = (float)(currentFrameTime - lastFrameTime) / CLOCKS_PER_SEC;

        // Call Worker Update. This calls Update on all the available tasks on this Worker
        worker.Update(delta);

        lastFrameTime = currentFrameTime;
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

AsyncoTaskHandle& AsyncoTaskManager::AddTask(AsyncoTask* task, OnAsyncoTaskCompleted const& callback)
{
    AsyncoTaskBundle* bundle = new AsyncoTaskBundle;
    bundle->m_task = task;
    bundle->m_handle = new AsyncoTaskHandle(++m_taskHandleNum);
    bundle->m_completionCallback = callback;

    bundle->m_task->m_handleId = bundle->m_handle->GetHandleId();

    {
        std::lock_guard<std::mutex> guard(m_pendingTasksLock);
        m_pendingTasks.push(bundle);
    }

    return *bundle->m_handle;
}

void AsyncoTaskManager::AddCompletedTask(AsyncoTaskBundle* bundle)
{
    std::lock_guard<std::mutex> lock(m_completedTasksLock);
    m_completedTasks.push_back(bundle);
}

void AsyncoTaskManager::Update()
{
    {
        std::lock_guard<std::mutex> lock(m_completedTasksLock);

        if(m_completedTasks.size() > 0)
        {
            for (auto bundle : m_completedTasks)
            {
                bundle->m_completionCallback(bundle->m_task->GetResult());

                delete bundle->m_task;
                delete bundle->m_handle;
                delete bundle;
            }

            m_completedTasks.clear();
        }
    }
}
