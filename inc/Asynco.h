#pragma once

#ifdef ASYNCO_BUILD_LIBRARY
#define ASYNCO_EXPORT __declspec(dllexport)
#else
#define ASYNCO_EXPORT __declspec(dllimport)
#endif

#define ASYNCO_DEFINE_SINGLETON(className)              \
    private:                                            \
    static className* m_sInstance;                      \
    public:                                             \
    ASYNCO_EXPORT static className* GetInstance();      \

#define ASYNCO_IMPLEMENT_SINGLETON(className)                  \
    className* className::m_sInstance = nullptr;        \
    ASYNCO_EXPORT className* className::GetInstance()   \
    {                                                   \
        if(!m_sInstance)                                \
        {                                               \
            m_sInstance = new className();              \
        }                                               \
        return m_sInstance;                             \
    }
                    

    
