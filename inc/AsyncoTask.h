#include "Asynco.h"

enum class ETaskResult
{
    InProgress,
    Success,
    Failure
};

class AsyncoTask
{
public:
    ASYNCO_EXPORT AsyncoTask();
    ASYNCO_EXPORT virtual ~AsyncoTask();

    ASYNCO_EXPORT virtual void          Start() {};
    ASYNCO_EXPORT virtual ETaskResult   Update(float delta) { return ETaskResult::Failure; };

    ASYNCO_EXPORT AsyncoTaskResult*     GetResult();

    uint32                              GetHandleId() { return m_handleId; }

protected:
    ASYNCO_EXPORT void                                SetResult(AsyncoTaskResult* result);

private:
    friend class AsyncoTaskManager;

    uint32                              m_handleId;

    AsyncoTaskResult*                   m_result;
};
