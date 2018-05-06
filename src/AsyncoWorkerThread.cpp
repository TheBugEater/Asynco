#include "AsyncoWorkerThread.h"
#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"

AsyncoWorkerThread::AsyncoWorkerThread()
{

}

void AsyncoWorkerThread::AssignTask(AsyncoTaskBundle* bundle)
{
    if(bundle)
    {
        m_taskBundles.push_back(bundle);

        bundle->m_task->Start();
    }
}

void AsyncoWorkerThread::Update(float delta)
{
    auto it = m_taskBundles.begin();
    while(it != m_taskBundles.end())
    {
        auto bundle = *it;
        if(bundle->m_task->Update(delta) > ETaskResult::InProgress)
        {
            AsyncoTaskManager::GetInstance()->AddCompletedTask(bundle);
            it = m_taskBundles.erase(it);
        }
        else
        {
            it++;
        }
    }
}

uint32 AsyncoWorkerThread::GetCurrentNumTasks() const
{
    return m_taskBundles.size();
}
