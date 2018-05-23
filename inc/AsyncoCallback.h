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
        m_holder = new ClassHolder<TClass>(obj, func);
    }

    AsyncoCallback(TRet (*func)(Args... args))
    {
        m_holder = new StaticHolder(func);
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

    // Holder to Handle Class member functions
    template<typename TClass>
    struct ClassHolder : public BaseHolder
    {
        typedef TRet(TClass::*FuncPointer)(Args... args);

        ClassHolder()
            : m_this(nullptr)
            , m_function(nullptr)
        {
        }

        ClassHolder(TClass* obj, FuncPointer func)
            : m_this(obj)
            , m_function(func)
        {
        }

        virtual BaseHolder* Clone()
        {
            ClassHolder<TClass>* holder = new ClassHolder<TClass>();
            holder->m_this = m_this;
            holder->m_function = m_function;
            return holder;
        }

        virtual TRet Invoke(Args... args)
        {
            if(!m_this)
            {
                return TRet();
            }

            return (m_this->*m_function)(args...);
        }

        TClass*             m_this;
        FuncPointer         m_function;
    };

    // Holder which Handles static functions
    struct StaticHolder : public BaseHolder
    {
        typedef TRet(*FuncPointer)(Args... args);

        StaticHolder()
            : m_function(nullptr)
        {
        }

        StaticHolder(FuncPointer func)
            : m_function(func)
        {
        }

        virtual BaseHolder* Clone()
        {
            StaticHolder* holder = new StaticHolder();
            holder->m_function = m_function;
            return holder;
        }

        virtual TRet Invoke(Args... args)
        {
            if(!m_function)
            {
                return TRet();
            }

            return m_function(args...);
        }

        FuncPointer         m_function;
    };

    TRet operator() (Args... args)
    {
        return m_holder->Invoke(args...);
    }

private:

    BaseHolder*         m_holder;
};
