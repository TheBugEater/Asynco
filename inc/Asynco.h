#pragma once

#define ASYNCO_DEFINE_SINGLETON(className)                     \
    private:                                            \
    static className* m_sInstance;                      \
    public:                                             \
    static className* GetInstance();                    \

#define ASYNCO_IMPLEMENT_SINGLETON(className)                  \
    className* className::m_sInstance = nullptr;        \
    className* className::GetInstance()                 \
    {                                                   \
        if(!m_sInstance)                                \
        {                                               \
            m_sInstance = new className();              \
        }                                               \
        return m_sInstance;                             \
    }
                    

    
