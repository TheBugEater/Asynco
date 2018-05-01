#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"

ASYNCO_IMPLEMENT_SINGLETON(AsyncoTaskManager)

AsyncoTaskManager::AsyncoTaskManager()
{
}

void AsyncoTaskManager::RunInTheThread()
{
    while(true)
    {
        AsyncoTask* currentJob = nullptr;
        {
            std::lock_guard<std::mutex> guard(m_queueLock);
            if(!m_taskQueue.empty())
            {
                currentJob = m_taskQueue.front();
                m_taskQueue.pop();
                std::cout << "Picked a New Job for Thread: " << std::this_thread::get_id() << "\n";
            }
        }

        if(currentJob)
        {
            currentJob->DoInBackground();
        }
    }
}

void AsyncoTaskManager::Start(unsigned int maxThreads)
{
    for(unsigned int i = 0; i < maxThreads; i++)
    {
        m_threadPool.push_back(std::thread(&AsyncoTaskManager::RunInTheThread, this));
    }
}

void AsyncoTaskManager::AddTask(AsyncoTask* task)
{
    std::lock_guard<std::mutex> guard(m_queueLock);
    m_taskQueue.push(task);
}
