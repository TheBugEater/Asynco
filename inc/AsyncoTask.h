#include "Asynco.h"

enum class ETaskResult
{
    None,
    Success,
    Failure
};

class AsyncoTask
{
public:
    AsyncoTask();

    ASYNCO_EXPORT virtual void          Start() {};
    ASYNCO_EXPORT virtual ETaskResult   Update(float delta) { return ETaskResult::Failure; };

    ASYNCO_EXPORT AsyncoTaskResult*     GetResult();

protected:
    void                                SetResult(AsyncoTaskResult* result);

private:
    uint32                              m_handleId;

    AsyncoTaskResult*                   m_result;
};
