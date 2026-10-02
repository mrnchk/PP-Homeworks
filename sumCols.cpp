#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdint>

std::int64_t sumCols(const std::vector<std::vector<int>>& m)
{
    std::int64_t sum = 0;

    for (size_t j = 0; j < m.size(); j++) {
        for (size_t i = 0; i < m.size(); i++) {
            sum += m[i][j];
        }
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
        result += sumCols(m);
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