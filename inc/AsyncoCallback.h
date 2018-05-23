#pragma once
#include "Asynco.h"

template<typename TRet>
class AsyncoCallback;

template<typename TRet, typename ... Args>
class AsyncoCallback <TRet(Args...)>
{
public:
    AsyncoCallback()
        : m_holder(nullptr)
    {
    }

    AsyncoCallback(AsyncoCallback&& callback)
    {
        m_holder = callback.m_holder;
        callback.m_holder = nullptr;
    }

    AsyncoCallback& operator=(AsyncoCallback&& callback)
    {
        ASYNCO_SAFE_DELETE(m_holder);

        m_holder = callback.m_holder;
        callback.m_holder = nullptr;
        return *this;
    }

    AsyncoCallback(AsyncoCallback& callback)
    {
        if(callback.m_holder)
        {
            m_holder = callback.m_holder->Clone();
        }
    }

    AsyncoCallback& operator=(AsyncoCallback const& callback)
    {
        ASYNCO_SAFE_DELETE(m_holder);

        if(callback.m_holder)
        {
            m_holder = callback.m_holder->Clone();
        }
        return *this;
    }
   
    template<typename TClass>
    AsyncoCallback(TClass* obj, TRet(TClass::*func)(Args... args))
    {
        m_holder = new ThisHolder<TClass>(obj, func);
    }

    ~AsyncoCallback()
    {
        ASYNCO_SAFE_DELETE(m_holder);
    }

    struct BaseHolder
    {
        virtual BaseHolder* Clone() = 0;
        virtual TRet Invoke(Args...) = 0;
    };

    template<typename TClass>
    struct ThisHolder : public BaseHolder
    {
        typedef TRet(TClass::*FuncPointer)(Args... args);

        ThisHolder()
            : m_this(nullptr)
            , m_function(nullptr)
        {
        }

        ThisHolder(TClass* obj, FuncPointer func)
            : m_this(obj)
            , m_function(func)
        {
        }

        virtual BaseHolder* Clone()
        {
            ThisHolder<TClass>* holder = new ThisHolder<TClass>();
            holder->m_this = m_this;
            holder->m_function = m_function;
            return holder;
        }

        virtual TRet Invoke(Args... args)
        {
            return (m_this->*m_function)(args...);
        }

        TClass*             m_this;
        FuncPointer         m_function;
    };

    TRet operator() (Args... args)
    {
        return m_holder->Invoke(args...);
    }

private:

    BaseHolder*         m_holder;
};
