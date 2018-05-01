#include "Asynco.h"

typedef void(*OnAsyncTaskDone)();

class AsyncoTask
{
public:
    ASYNCO_EXPORT virtual void PreExecute() {};
    ASYNCO_EXPORT virtual void DoInBackground() {};
    ASYNCO_EXPORT virtual void PostExecute() {};
};
