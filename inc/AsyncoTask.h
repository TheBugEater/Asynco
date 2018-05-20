#include "Asynco.h"

class AsyncoTask
{
public:
    ASYNCO_EXPORT AsyncoTask();
    ASYNCO_EXPORT virtual ~AsyncoTask();

    ASYNCO_EXPORT virtual void          Start() {};
    ASYNCO_EXPORT virtual ETaskStatus   Update(float delta) { return ETaskStatus::Failed; };

    ASYNCO_EXPORT AsyncoTaskResult*     GetResult();

    uint32                              GetHandleId() { return m_handleId; }

protected:
    ASYNCO_EXPORT void                  SetResult(AsyncoTaskResult* result);

private:
    friend class AsyncoTaskManager;

    uint32                              m_handleId;

    AsyncoTaskResult*                   m_result;
};
