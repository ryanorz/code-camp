#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <stdexcept>

using namespace std;

mutex g_lock_cout;
mutex g_lock_queue;
condition_variable g_queuecheck;
queue<exception_ptr> g_exceptions;
bool g_workerdone = false;

void worker(int id)
{
    try {
        std::this_thread::sleep_for(std::chrono::seconds(id));
        throw runtime_error("something wrong, id = " + to_string(id));
    } catch(...) {
        lock_guard<mutex> lock(g_lock_queue);
        g_exceptions.push(current_exception());
        g_queuecheck.notify_one();
    }
}

void logger()
{
    while (!g_workerdone) {
        std::unique_lock<mutex> locker(g_lock_queue);
        g_queuecheck.wait(locker, [&]() {
            // 加入条件函数避免虚假唤醒
            return !g_exceptions.empty() || g_workerdone;
        });
        while (!g_exceptions.empty()) {
            try {
                auto ep = g_exceptions.front();
                if (ep != nullptr)
                    rethrow_exception(ep);
            } catch (const exception &e) {
                std::unique_lock<mutex> cout_locker(g_lock_cout);
                std::cout << "[logger] exception: " << e.what() << endl;
            }
            g_exceptions.pop();
        }
    }
}

int main() {
    thread t_logger(logger);
    vector<thread> t_workers;
    for (int i = 0; i < 5; ++i) {
        t_workers.push_back(thread(worker, i));
    }
    for (auto &t_worker : t_workers)
        t_worker.join();
    {
        lock_guard<mutex> lock(g_lock_queue);
        g_workerdone = true;
        g_queuecheck.notify_one();
    }
    t_logger.join();

    return 0;
}
