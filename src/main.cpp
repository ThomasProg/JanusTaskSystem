#include <Task.hpp>
#include <iostream>
#include <functional>

class A
{
public:
    int i = 1;

    A() = default;
    A(const A&)
    {
        std::cout << "copy" << std::endl;
    }

    A(A&& a)
    {
        a.i = 0;
        std::cout << "move" << std::endl;
    }

    A& operator=(const A&)
    {
        std::cout << "copy" << std::endl;
        return *this;
    }

    A& operator=(A&& a)
    {
        a.i = 0;
        std::cout << "move" << std::endl;
        return *this;
    }

    void f(std::string s)
    {
        std::cout << "member function " << s << std::endl;
    }

    void operator()(A a)
    {
        std::cout << "operator()(A)" << std::endl;
    }

    void operator()()
    {
        std::cout << "operator()()" << std::endl;
    }
};













void test(int i)
{
    std::cout << "C function " << i << std::endl;
}


void runTests()
{
    // C Function
    {
        // FnTask<void(int)> t (test, 3);
        auto t = makeFnTask(test, 3);
        Task<>* aTask = &t;
        aTask->invoke();
    }    

    // Member Function
    {
        A a;
        auto t = makeMemFnTask(&a, &A::f, std::string("Hello"));
        Task<>* aTask = &t;
        aTask->invoke();
    }

    // Lambda
    {
        int a = 3;
        auto c = [a](){std::cout << "hello " << a << '\n';};
        auto t = makeFunctorTask(c);
        Task<>* aTask = &t;
        aTask->invoke();
    }

    // Functor
    {
        A a;
        auto t = makeFunctorTask(a);
        Task<>* aTask = &t;
        aTask->invoke();
    }
}

#include <vector>










void ArgsTest()
{
    A a;
    A b;

    std::cout << "=== My Version ===" << std::endl;
    FunctorTask<A, void(A)> t (std::move(a), std::move(b));

    Task<>* typeErased = &t;
    typeErased->invokeFinal(); 

}

















int main()
{
    runTests();
    ArgsTest();
}