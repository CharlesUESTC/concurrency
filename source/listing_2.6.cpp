#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

class ScopedThread
{
public:
    explicit ScopedThread(std::thread t): t_(std::move(t))
    {
        if (!t_.joinable())
        {
            throw std::logic_error("No thread");
        }
    }
    ~ScopedThread()
    {
        t_.join();
    }
    ScopedThread(const ScopedThread &) = delete;
    ScopedThread &operator=(const ScopedThread &) = delete;

private:
    std::thread t_;
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
            doSomething(i_); // Potential access to dangling reference
        }
        std::cout << "New thread ends.\n";
    }
};

void f()
{
    int some_local_state = 0;
    ScopedThread t{std::thread{Func(some_local_state)}};
    doSomethingInCurrentThread();
}

int main()
{
    f();
    std::cout << "Current thread ends." << std::endl;

    return 0;
}
