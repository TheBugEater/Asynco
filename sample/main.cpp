#include <stdio.h>
#include "AsyncoTaskManager.h"
#include "AsyncoTask.h"

#ifdef __linux
#include <unistd.h>
#endif

class AsyncTestTask : public AsyncoTask
{
    virtual void DoInBackground() 
    { 
        std::cout << "Thread : " << std::this_thread::get_id() << std::endl;

#ifdef __linux
        sleep(1);
#endif
    }
};

int main()
{
    auto instance = AsyncoTaskManager::GetInstance();
    
    // Max Worker Threads
    instance->Start(4);

    // Spawn 100 jobs to test
    for(int i=0; i < 100; i++)
    {
        instance->AddTask(new AsyncTestTask());
    }

    while(1){}

    return 0;
}

