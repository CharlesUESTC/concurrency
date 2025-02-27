#include <algorithm>
#include <iostream>
#include <list>
#include <mutex>

std::list<int> some_list;
std::mutex some_mutex;

void addToList(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool listContains(int value_to_find)
{
    std::lock_guard guard(some_mutex);
    return std::find(some_list.cbegin(), some_list.cend(), value_to_find) != some_list.cend();
}

int main()
{
    addToList(42);
    std::cout << "contains(1)=" << listContains(1) << ", contains(42)=" << listContains(42) << std::endl;

    return 0;
}
