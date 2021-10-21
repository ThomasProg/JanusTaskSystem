#pragma once

#include <tuple>

template<typename R = void>
class Task
{
public:
    virtual R operator()() = 0;
    inline R invoke()
    {
        return (*this)();
    }
    virtual R invokeFinal()
    {
        return invoke();
    }
};

template<typename T>
class FnTask;

template<typename R, typename ... ARGS>
class FnTask<R(ARGS...)> final : public Task<R>
{
    using F_TYPE = R(ARGS...);
    F_TYPE* ptr;
    std::tuple<ARGS...> args;

public:
    template<typename ... ARGS2>
    FnTask(F_TYPE* ptr2, ARGS2&& ... args2) noexcept(noexcept(std::tuple<ARGS...>(std::forward<ARGS2>(args2)...)))
        : ptr(ptr2), args(std::forward<ARGS2>(args2)...) 
    {

    }

    R operator()() noexcept(noexcept(std::apply(ptr, args)))
    {
        return std::apply(ptr, args);
    }
};

template<typename R, typename ... ARGS>
FnTask<R(ARGS...)> makeFnTask(R (*ptr2)(ARGS...), ARGS&& ... args2) noexcept(noexcept(FnTask<R(ARGS...)>(ptr2, std::forward<ARGS>(args2)...)))
{
    return FnTask<R(ARGS...)>(ptr2, std::forward<ARGS>(args2)...);
}

template<typename CLASS, typename T>
class MemFnTask;

template<typename CLASS, typename R, typename ... ARGS>
class MemFnTask<CLASS, R(ARGS...)> final : public Task<R>
{
    using F_PTR = R(CLASS::*)(ARGS...);
    F_PTR ptr;
    std::tuple<CLASS*, ARGS...> args;

    template<typename F_PTR, typename R>
    class MemFn
    {
        F_PTR ptr;
    
    public:
        MemFn(F_PTR ptr2) noexcept
            : ptr(ptr2)
        {

        }

        template<typename ... ARGS2>
        R operator()(CLASS* instance, ARGS2&& ... args) noexcept(noexcept((instance->*ptr)(args...)))
        {
            return (instance->*ptr)(std::forward<ARGS2>(args)...);
        }
    };

public:    
    template<typename ... ARGS2>
    MemFnTask(CLASS* instance2, F_PTR ptr2, ARGS2&& ... args2) noexcept(noexcept(std::tuple<CLASS*, ARGS...>(instance2, std::forward<ARGS2>(args2)...)))
        : ptr(ptr2), args(instance2, std::forward<ARGS2>(args2)...) 
    {

    }

    R operator()() noexcept(noexcept(std::apply(MemFn<F_PTR, R>(ptr), args)))
    {
        return std::apply(MemFn<F_PTR, R>(ptr), args);
    }
};

template<typename CLASS, typename R, typename ... ARGS>
MemFnTask<CLASS, R(ARGS...)> makeMemFnTask(CLASS* instance2, R(CLASS::* ptr2)(ARGS...), ARGS&& ... args2) noexcept(noexcept(MemFnTask<CLASS, R(ARGS...)>(instance2, ptr2, std::forward<ARGS>(args2)...)))
{
    return MemFnTask<CLASS, R(ARGS...)>(instance2, ptr2, std::forward<ARGS>(args2)...);
}




template<typename FUNCTOR, typename T>
class FunctorTask;

template<typename FUNCTOR,typename R, typename ... ARGS>
class FunctorTask<FUNCTOR, R(ARGS...)> final : public Task<R>
{
    FUNCTOR functor;
    std::tuple<ARGS...> args;

public:
    template<typename FUNCTOR2, typename ... ARGS2>
    FunctorTask(FUNCTOR2&& functor2, ARGS2&& ... args2) noexcept(noexcept(std::tuple<ARGS...>(std::forward<ARGS2>(args2)...)))
        : functor(std::forward<FUNCTOR2>(functor2)), args(std::forward<ARGS2>(args2)...) 
    {

    }

    R operator()() noexcept(noexcept(std::apply(functor, args)))
    {
        return std::apply(functor, args);
    }

    void invokeFinal() noexcept(noexcept(std::apply(functor, std::move(args)))) override final 
    {
        return std::apply(functor, std::move(args));
    }
};

template<typename FUNCTOR, typename ... ARGS>
auto makeFunctorTask(FUNCTOR&& functor, ARGS&& ... args) noexcept(noexcept(FunctorTask<FUNCTOR, decltype(functor(args...)) (ARGS...)>(std::forward<FUNCTOR>(functor), std::forward<ARGS>(args) ...)))
{
    return FunctorTask<FUNCTOR, decltype(functor(args...)) (ARGS...)>(std::forward<FUNCTOR>(functor), std::forward<ARGS>(args) ...);
}