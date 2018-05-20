#pragma once
#include "ControlFlow/AsyncoControlFlow.h"

class AsyncoAuto : public AsyncoControlFlow
{
public:

protected:
    virtual void        Execute();
    virtual void        TaskCompleted(AsyncoTask* task);
};
