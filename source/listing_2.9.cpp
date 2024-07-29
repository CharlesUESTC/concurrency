#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

template <typename Iterator, typename T>
struct AccumulateBlock
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init)
{
    const unsigned long length = static_cast<unsigned long>(std::distance(first, last));
    if (!length)
    {
        return init;
    }

    const unsigned long min_per_thread = 25;
    const unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    const unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    const unsigned long block_size = length / num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(AccumulateBlock<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }
    AccumulateBlock<Iterator, T>()(block_start, last, results[num_threads - 1]);

    for (auto &entry : threads)
    {
        entry.join();
    }

    return std::accumulate(results.begin(), results.end(), init);
}

int main()
{
    std::vector<int> vi;
    for (int i = 0; i < 150; ++i)
    {
        vi.push_back(i + 1);
    }

    int sum = parallelAccumulate(vi.begin(), vi.end(), 5);
    std::cout << "sum = " << sum << std::endl;

    return 0;
}
