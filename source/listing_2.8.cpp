// #include <algorithm>
// #include <functional>
#include <iostream>
#include <thread>
#include <vector>

void doWork(unsigned /* id */)
{
    std::cout << "New thread ends.\n";
}

void f()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
    {
        threads.emplace_back(doWork, i);
    }
    for (auto &entry : threads)
    {
        entry.join();
    }
    // std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main()
{
    f();
    std::cout << "Main thread ends." << std::endl;

    return 0;
}
