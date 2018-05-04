#pragma once

class AsyncoTask;

/************************************************************
* Every Worker Thread will own an object of AsyncoWorkerThread
************************************************************/
class AsyncoWorkerThread
{
public:
    AsyncoWorkerThread();

    void            AssignTask(AsyncoTask* task);

    void            Update(float delta);

private:
    AsyncoTask*     m_currentTask;
};
