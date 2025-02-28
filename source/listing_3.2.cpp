#include <iostream>
#include <mutex>
#include <string>

class SomeData
{
    int a_ = 0;
    std::string b_;

public:
    void doSomething() { a_ = 1; }
    int getA() const { return a_; }
};

class DataWrapper
{
private:
    SomeData data_;
    std::mutex m_;

public:
    template <typename Function>
    void processData(Function func)
    {
        std::lock_guard<std::mutex> l(m_);
        func(data_);
    }

    int readDataA()
    {
        std::lock_guard<std::mutex> l(m_);
        return data_.getA();
    }
};

SomeData *unprotected;

void maliciousFunction(SomeData &protected_data)
{
    unprotected = &protected_data;
}

DataWrapper x;

void foo()
{
    x.processData(maliciousFunction);
    unprotected->doSomething();
}

int main()
{
    std::cout << "x.a_ = " << x.readDataA() << std::endl;
    foo();
    std::cout << "x.a_ = " << x.readDataA() << std::endl;

    return 0;
}
