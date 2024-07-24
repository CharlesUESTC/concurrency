#include <cstdio>
#include <thread>

class ThreadGuard
{
public:
    explicit ThreadGuard(std::thread &t): t_(t) { }
    ~ThreadGuard()
    {
        if (t_.joinable())
        {
            t_.join();
        }
    }
    ThreadGuard(const ThreadGuard &) = delete;
    ThreadGuard &operator=(const ThreadGuard &) = delete;

private:
    std::thread &t_;
};

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
            doSomething(i_);
        }
        printf("New thread ends.\n");
    }
};

void f()
{
    int some_local_state = 0;
    Func my_func(some_local_state);
    std::thread t(my_func);
    ThreadGuard g(t);

    doSomethingInCurrentThread();
}

int main()
{
    f();
    printf("Current thread ends.\n");

    return 0;
}
