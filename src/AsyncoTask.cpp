#include "AsyncoTask.h"

AsyncoTask::AsyncoTask()
    : m_handleId(0)
    , m_result(nullptr)
{

}

AsyncoTask::~AsyncoTask()
{
    if(m_result)
    {
        delete m_result;
        m_result = nullptr;
    }
}

AsyncoTaskResult* AsyncoTask::GetResult()
{
    return m_result;
}

void AsyncoTask::SetResult(AsyncoTaskResult* result)
{
    m_result = result;
}


