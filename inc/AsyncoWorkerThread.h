#pragma once
#include "Asynco.h"
#include <vector>

class AsyncoTaskBundle;

/************************************************************
* Every Worker Thread will own an object of AsyncoWorkerThread
************************************************************/
class AsyncoWorkerThread
{
public:
    AsyncoWorkerThread();

    void                            AssignTask(AsyncoTaskBundle* bundle);

    void                            Update(float delta);

    uint32                          GetCurrentNumTasks() const;

private:
    std::vector<AsyncoTaskBundle*>  m_taskBundles;
};
