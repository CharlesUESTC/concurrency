#include <iostream>
#include <thread>

void doSomething(int &i)
{
    ++i;
}

void doSomethingInCurrentThread() { }

struct Func
{
    int &i_;
    Func(int &i): i_(i) { }
    void operator()()
    {
        for (unsigned j = 0; j < 1000000; ++j)
        {
            doSomething(i_); // Potential access to dangling reference
        }
        std::cout << "New thread ends.\n";
    }
};

void f()
{
    int some_local_state = 0;
    Func my_func(some_local_state);
    std::thread t(my_func);
    try
    {
        doSomethingInCurrentThread();
    }
    catch (...)
    {
        t.join();
        throw;
    }
    t.join();
    std::cout << "Var \'some_local_state\': " << some_local_state << "\n";
    std::cout << "Current thread ends." << std::endl;
}

int main()
{
    f();
    return 0;
}
