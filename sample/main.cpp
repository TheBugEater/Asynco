#include <stdio.h>
#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"

#ifdef _WIN32
#include <Windows.h>
#endif

uint32 count = 0;
std::mutex countMutex;

class AsyncTestTaskResult : public AsyncoTaskResult
{
public:
    AsyncTestTaskResult(uint32 val) : m_value(val) {}

    uint32          GetValue() { return m_value; }
private:
    uint32          m_value;
};

class AsyncTestTask : public AsyncoTask
{
    virtual ETaskResult Update(float delta) 
    { 
        {
            std::lock_guard<std::mutex> lock(countMutex);
            count++;
            std::cout << GetHandleId() << " : " << count << std::endl;

            SetResult(new AsyncTestTaskResult(count));
        }

        return ETaskResult::Success;
    }
};

void OnCompleted(AsyncoTaskResult* result)
{
    if(result)
    {
        auto testResult = result->GetResult<AsyncTestTaskResult>();

        std::lock_guard<std::mutex> lock(countMutex);
        std::cout << "Finished Task: " << testResult->GetValue() << std::endl;
    }
}

int main(int argc, char** argv)
{
    auto instance = AsyncoTaskManager::GetInstance();
    
    uint32 numThreads = std::thread::hardware_concurrency();
    uint32 tasksPerThread = 8;

    if(argc > 2)
    {
        numThreads = atoi(argv[1]);
        tasksPerThread = atoi(argv[2]);
    }

    // Max Worker Threads
    instance->Start(numThreads, tasksPerThread);

    uint32 num;
    std::cin >> num;
    for (uint32 i = 0; i < num; i++)
    {
        instance->AddTask(new AsyncTestTask(), OnCompleted);
    }

    while (1)
    {
        instance->Update();
    }

    return 0;
}

