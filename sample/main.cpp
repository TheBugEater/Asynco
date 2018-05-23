#include <stdio.h>
#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"

#include <ctime>

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
    virtual void Start()
    {
        m_destroyTime = (float)(std::rand() % 10);
    }

    virtual ETaskStatus Update(float delta) 
    { 
        m_destroyTime -= delta;
        if (m_destroyTime > 0)
        {
            return ETaskStatus::InProgress;
        }

        {
            std::lock_guard<std::mutex> lock(countMutex);
            count++;
            std::cout << "Task Handle: " << GetHandleId() << " : " << count << std::endl;

            SetResult(new AsyncTestTaskResult(count));
        }

        return ETaskStatus::Success;
    }
private:
    float  m_destroyTime;
};

class HandleResult
{
public:
    void OnCompleted(AsyncoTaskResult* result)
    {
        if(result)
        {
            auto testResult = result->GetResult<AsyncTestTaskResult>();
            if(testResult)
            {
                std::lock_guard<std::mutex> lock(countMutex);
                std::cout << "Class Function: Finished Task: " << testResult->GetValue() << std::endl;
            }
        }
    }
};

void OnCompleted(AsyncoTaskResult* result)
{
    if(result)
    {
        auto testResult = result->GetResult<AsyncTestTaskResult>();
        if(testResult)
        {
            std::lock_guard<std::mutex> lock(countMutex);
            std::cout << "Static Function : Finished Task: " << testResult->GetValue() << std::endl;
        }
    }
}

int main(int argc, char** argv)
{
    // Seed the Random Number Generator
    std::srand((unsigned int)std::time(nullptr)); 

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

    HandleResult result;
    uint32 num;
    std::cin >> num;
    for (uint32 i = 0; i < num; i++)
    {
//        instance->AddTask(new AsyncTestTask(), OnAsyncoTaskCompleted(OnCompleted));
         instance->AddTask(new AsyncTestTask(), OnAsyncoTaskCompleted(&result, &HandleResult::OnCompleted));
    }

    while (true)
    {
        instance->Update();
    }

    return 0;
}

