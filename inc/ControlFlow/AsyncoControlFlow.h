#pragma once
#include "Asynco.h"

class AsyncoTask;

class AsyncoControlFlow
{
public:
    AsyncoControlFlow() {}
    virtual ~AsyncoControlFlow() {}

protected:
    virtual void        Execute() = 0;
    virtual void        TaskCompleted(AsyncoTask* task) = 0;
};
