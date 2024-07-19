#include <cstdio>
#include <thread>

class thread_guard
{
public:
    explicit thread_guard(std::thread &t): t_(t) { }
    ~thread_guard()
    {
        if (t_.joinable())
        {
            t_.join();
        }
    }
    thread_guard(const thread_guard &) = delete;
    thread_guard &operator=(const thread_guard &) = delete;

private:
    std::thread &t_;
};

void doSomething(int &i)
{
    ++i;
}

void doSomethingInCurrentThread() { }

struct func
{
    int &i_;

    func(int &i): i_(i) { }

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
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);

    doSomethingInCurrentThread();
}

int main()
{
    f();
    printf("Current thread ends.\n");

    return 0;
}
