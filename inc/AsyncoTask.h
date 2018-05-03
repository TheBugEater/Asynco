#include "Asynco.h"

class AsyncoTask
{
public:
    AsyncoTask() : m_handleId(0) {}

    ASYNCO_EXPORT virtual void      DoInBackground() {};

private:
    uint32                          m_handleId;
};
