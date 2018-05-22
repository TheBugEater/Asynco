#pragma once

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

    template<typename TClass, typename ... Args>
    AsyncoCallback(TClass* obj, TRet(TClass::*func)(Args... args))
    {
        m_holder = new ThisHolder<TClass>(obj, func);
    }

    ~AsyncoCallback()
    {
        if (m_holder)
        {
            delete m_holder;
            m_holder = nullptr;
        }
    }

    struct BaseHolder
    {
        virtual TRet Invoke(Args...) = 0;
    };

    template<typename TClass>
    struct ThisHolder : public BaseHolder
    {
        typedef TRet(TClass::*FuncPointer)(Args... args);

        ThisHolder(TClass* obj, FuncPointer func)
            : m_this(obj)
            , m_function(func)
        {
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