#pragma once
#include "ControlFlow/AsyncoControlFlow.h"

class AsyncoAuto : public AsyncoControlFlow
{
protected:
    virtual void        Execute();
    virtual void        TaskCompleted(AsyncoTask* task);
};
