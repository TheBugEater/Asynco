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
        auto status = bundle->m_task->Update(delta);
        if(status > ETaskStatus::InProgress)
        {
            auto result = bundle->m_task->GetResult();
            if(result)
            {
                result->SetStatus(status);
            }

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
    return (uint32)m_taskBundles.size();
}
