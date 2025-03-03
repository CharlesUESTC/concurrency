#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <utility>

struct EmptyStack : std::exception
{
    const char *what() const noexcept
    {
        return "empty stack";
    }
};

template <typename T>
class ThreadsafeStack
{
private:
    std::stack<T> data_;
    mutable std::mutex m_;

public:
    ThreadsafeStack() { }
    ThreadsafeStack(const ThreadsafeStack &other)
    {
        std::lock_guard lock(other.m_);
        data_ = other.data_;
    }
    ThreadsafeStack &operator=(const ThreadsafeStack &) = delete;

    void push(T new_value)
    {
        std::lock_guard lock(m_);
        data_.push(std::move(new_value));
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard lock(m_);
        if (data_.empty())
        {
            throw EmptyStack();
        }
        const std::shared_ptr<T> res(std::make_shared<T>(data_.top()));
        data_.pop();
        return res;
    }

    void pop(T &value)
    {
        std::lock_guard lock(m_);
        if (data_.empty())
        {
            throw EmptyStack();
        }
        value = data_.top();
        data_.pop();
    }

    bool empty() const
    {
        std::lock_guard lock(m_);
        return data_.empty();
    }
};

int main()
{
    ThreadsafeStack<int> si;
    // si.pop();
    si.push(5);
    si.pop();
    si.push(7);
    if (!si.empty())
    {
        int x;
        si.pop(x);
        // std::cout << "x = " << x << std::endl;
    }

    return 0;
}
