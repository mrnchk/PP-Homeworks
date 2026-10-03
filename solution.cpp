#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

std::mutex mtx;
std::condition_variable cv;

int h_count = 0;
int o_count = 0;

void hydrogen()
{
    std::unique_lock<std::mutex> lock(mtx);

    while (h_count == 2) {
        cv.wait(lock);
    }

    std::cout << "H";
    h_count++;

    if (h_count == 2 && o_count == 1) {
        h_count = 0;
        o_count = 0;
    }

    lock.unlock();
    cv.notify_all();
}

void oxygen()
{
    std::unique_lock<std::mutex> lock(mtx);

    while (o_count == 1) {
        cv.wait(lock);
    }

    std::cout << "O";
    o_count++;

    if (h_count == 2 && o_count == 1) {
        h_count = 0;
        o_count = 0;
    }

    lock.unlock();
    cv.notify_all();
}

int main()
{
    std::thread t1([&]{ hydrogen(); });
    std::thread t2([&]{ hydrogen(); });
    std::thread t3([&]{ oxygen(); });
    std::thread t4([&]{ hydrogen(); });
    std::thread t5([&]{ oxygen(); });
    std::thread t6([&]{ hydrogen(); });

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    std::cout << std::endl;
}