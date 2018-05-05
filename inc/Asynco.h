#pragma once

// Unsigned base types.
typedef unsigned char       uint8;      // 8-bit  unsigned.
typedef unsigned short int  uint16;     // 16-bit unsigned.
typedef unsigned int        uint32;     // 32-bit unsigned.
typedef unsigned long long  uint64;     // 64-bit unsigned.

// Signed base types.
typedef	signed char         int8;       // 8-bit  signed.
typedef signed short int    int16;      // 16-bit signed.
typedef signed int          int32;      // 32-bit signed.
typedef signed long long    int64;      // 64-bit signed.

// Character types.
typedef char                ANSICHAR;   // An ANSI character       
typedef wchar_t             WIDECHAR;   // A wide character        

#ifdef _WIN64
#define PTR_TO_UINT uint64
#elif _WIN32
#define PTR_TO_UINT uint32
#endif

#ifdef _WIN32
#ifdef ASYNCO_BUILD_LIBRARY
#define ASYNCO_EXPORT __declspec(dllexport)
#else
#define ASYNCO_EXPORT __declspec(dllimport)
#endif
#else
#define ASYNCO_EXPORT 
#endif

#define ASYNCO_DEFINE_SINGLETON(className)              \
    private:                                            \
    static className* m_sInstance;                      \
    public:                                             \
    ASYNCO_EXPORT static className* GetInstance();      

#define ASYNCO_IMPLEMENT_SINGLETON(className)           \
    className* className::m_sInstance = nullptr;        \
    className* className::GetInstance()                 \
    {                                                   \
        if(!m_sInstance)                                \
        {                                               \
            m_sInstance = new className();              \
        }                                               \
        return m_sInstance;                             \
    }

enum class ETaskStatus
{
    Idle,
    InProgress,
    Completed
};

class AsyncoTaskResult
{
    template<typename T>
    T* GetResult()
    {
        return static_cast<T*>(this);
    }
};

// Function Pointer to notify Task Completion
typedef void(OnAsyncoTaskCompleted)(AsyncoTaskResult*);

/********************************************************
 *  AsyncoTaskHandle
 *  Every Task gets it's own handle to query the state
 * ******************************************************/
class AsyncoTaskHandle
{
    // Only AsyncoWorkerThread should be able to create handles and change status
    friend class    AsyncoWorkerThread;
    friend class    AsyncoTaskManager;

public:
    uint32          GetHandleId() const { return m_handleId; }
    ETaskStatus     GetStatus() const { return m_taskStatus; }

private:
    AsyncoTaskHandle(uint32 handleId);

    void                    SetStatus(ETaskStatus status) { m_taskStatus = status; }

    uint32                  m_handleId;
    ETaskStatus             m_taskStatus;
};

