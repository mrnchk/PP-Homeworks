#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

int data = 0;
bool ready = false;

int atomicData = 0;
std::atomic<bool> atomicReady(false);

std::atomic<int> errors(0);

void writer()
{
    data = 12;
    ready = true;
}

void reader()
{
    while (!ready) {
    }

    if (data != 12) {
        errors.fetch_add(1, std::memory_order_relaxed);
    }
}

void atomicWriter()
{
    atomicData = 12;
    atomicReady.store(true, std::memory_order_relaxed);
}

void atomicReader()
{
    while (!atomicReady.load(std::memory_order_relaxed)) {
    }

    if (atomicData != 12) {
        errors.fetch_add(1, std::memory_order_relaxed);
    }
}

int main()
{
    const int readersCount = 8;

    {
        std::vector<std::thread> readers;

        for (int i = 0; i < readersCount; i++) {
            readers.emplace_back(reader);
        }

        std::thread writerThread(writer);

        writerThread.join();

        for (auto& thread : readers) {
            thread.join();
        }

        std::cout << "Bool errors: "
                  << errors.load(std::memory_order_relaxed)
                  << std::endl;
    }

    errors.store(0, std::memory_order_relaxed);

    {
        std::vector<std::thread> readers;

        for (int i = 0; i < readersCount; i++) {
            readers.emplace_back(atomicReader);
        }

        std::thread writerThread(atomicWriter);

        writerThread.join();

        for (auto& thread : readers) {
            thread.join();
        }

        std::cout << "Atomic relaxed bool errors: "
                  << errors.load(std::memory_order_relaxed)
                  << std::endl;
    }

    return 0;
}