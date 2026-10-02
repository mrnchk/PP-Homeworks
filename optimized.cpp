#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>
#include <thread>

void sumPart(const std::vector<std::vector<int>>& m,
             size_t begin,
             size_t end,
             std::int64_t& result)
{
    std::int64_t sum = 0;

    for (size_t i = begin; i < end; i++) {
        for (size_t j = 0; j < m[i].size(); j++) {
            sum += m[i][j];
        }
    }

    result = sum;
}

std::int64_t sumOptimized(const std::vector<std::vector<int>>& m)
{
    const int threadCount = 4;

    std::vector<std::thread> threads;
    std::vector<std::int64_t> results(threadCount, 0);

    size_t rowsPerThread = m.size() / threadCount;

    for (int t = 0; t < threadCount; t++) {
        size_t begin = t * rowsPerThread;
        size_t end = (t == threadCount - 1)
                   ? m.size()
                   : begin + rowsPerThread;

        threads.emplace_back(
            sumPart,
            std::cref(m),
            begin,
            end,
            std::ref(results[t])
        );
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::int64_t sum = 0;

    for (int t = 0; t < threadCount; t++) {
        sum += results[t];
    }

    return sum;
}

int main()
{
    const int N = 2048;
    const int M = 1000;

    std::vector<std::vector<int>> m(N, std::vector<int>(N));

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 1000);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m[i][j] = dist(gen);
        }
    }

    auto start = std::chrono::steady_clock::now();

    std::int64_t result = 0;

    for (int i = 0; i < M; i++) {
        result += sumOptimized(m);
    }

    auto end = std::chrono::steady_clock::now();

    double time =
        std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "Sum: " << result / M << std::endl;
    std::cout << "Total time: " << time << " ms" << std::endl;
    std::cout << "Average time: " << time / M
              << " ms" << std::endl;

    return 0;
}