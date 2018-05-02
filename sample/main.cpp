#include <stdio.h>
#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"
#include <Windows.h>

uint32 count = 0;
std::mutex countMutex;

class AsyncTestTask : public AsyncoTask
{
    virtual void DoInBackground() 
    { 
        std::this_thread::sleep_for(std::chrono::seconds(1));

        {
            std::lock_guard<std::mutex> countMutex(countMutex);
            count++;
            std::cout << "Finished Tasks: " << count << std::endl;
        }
    }
};

int main()
{
    auto instance = AsyncoTaskManager::GetInstance();
    
    // Max Worker Threads
    instance->Start(std::thread::hardware_concurrency());

    while(1)
    {
        uint32 num;
        std::cin >> num;
        for(uint32 i=0; i < num; i++)
        {
            instance->AddTask(new AsyncTestTask());
        }
    }

    return 0;
}

