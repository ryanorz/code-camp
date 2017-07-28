
#include <stdarg.h>

#include <set>
#include <mutex>
#include <iostream>

template <typename T>
class ThreadSafeContainer
{
private:
    mutable std::recursive_mutex lock_;
    std::set<T> elements_;
public:
    void add(T element)
    {
        std::lock_guard<std::recursive_mutex> locker(lock_);
        elements_.insert(element);
    }
    void remove(T element)
    {
        std::lock_guard<std::recursive_mutex> locker(lock_);
        elements_.erase(element);
    }
    void addrange(int num, ...)
    {
        va_list arguments;
        va_start(arguments, num);
        for (int i = 0; i < num; i++) {
            std::lock_guard<std::recursive_mutex> locker(lock_);
            add(va_arg(arguments, T));
        }
        va_end(arguments);
    }
    void dump() const
    {
        std::lock_guard<std::recursive_mutex> locker(lock_);
        for (auto &e: elements_) {
            std::cout << e << std::endl;
        }
    }
    void exchange(ThreadSafeContainer<T> &cont, T element)
    {
        std::unique_lock<std::recursive_mutex> lock1(lock_, std::defer_lock);
        std::unique_lock<std::recursive_mutex> lock2(cont.lock_, std::defer_lock);
        std::lock(lock1, lock2);
        remove(element);
        cont.add(element);
    }
};
