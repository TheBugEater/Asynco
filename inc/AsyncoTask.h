#include "Asynco.h"

typedef void(*OnAsyncTaskDone)();

class AsyncoTask
{
    ASYNCO_EXPORT virtual void Start() {};
    ASYNCO_EXPORT virtual void Execute() {};
    ASYNCO_EXPORT virtual void Finish() {};
};