#include <chrono>
#include <iostream>
#include <thread>

void doSomething(int &i)
{
    ++i;
}

struct func
{
    int i_;
    func(int i): i_(i) { }
    // int &i_;
    // func(int &i): i_(i) { }
    void operator()()
    {
        for (unsigned j = 0; j < 1000000; ++j)
        {
            doSomething(i_); // Potential access to dangling reference
        }
        std::cout << "My thread ends.\n";
    }
};

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach(); // Don't wait for thread to finish
                        // New thread might still be running
}

int main()
{
    oops();
    std::cout << "Main thread ends.\n";
    // Give some time for detached thread to complete
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
