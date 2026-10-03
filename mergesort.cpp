#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

void merge(std::vector<int>& a, std::vector<int>& tmp, int left, int mid, int right) {
    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < right) tmp[k++] = a[j++];
    for (int p = left; p < right; ++p) a[p] = tmp[p];
}

void mergeSort(std::vector<int>& a, std::vector<int>& tmp, int left, int right) {
    if (right - left <= 1) return;
    int mid = left + (right - left) / 2;
    mergeSort(a, tmp, left, mid);
    mergeSort(a, tmp, mid, right);
    merge(a, tmp, left, mid, right);
}

void parallelMergeSort(std::vector<int>& a, std::vector<int>& tmp,
                       int left, int right, unsigned threads) {
    if (right - left <= 1) return;
    if (threads <= 1 || right - left < 100000) {
        mergeSort(a, tmp, left, right);
        return;
    }
    int mid = left + (right - left) / 2;
    unsigned leftThreads = threads / 2;
    unsigned rightThreads = threads - leftThreads;
    std::thread worker([&a, &tmp, left, mid, leftThreads]() {
        parallelMergeSort(a, tmp, left, mid, leftThreads);
    });
    parallelMergeSort(a, tmp, mid, right, rightThreads);
    worker.join();
    merge(a, tmp, left, mid, right);
}

int main() {
    unsigned threads = std::thread::hardware_concurrency();
    std::cout << "Threads count: " << threads << '\n';
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(-1'000'000, 1'000'000);
    for (int n : {1000, 10000, 100000, 500000, 1000000, 5000000}) {
        std::vector<int> original(n);
        for (int& x : original) {
            x = dist(gen);
        }
        double singleTotal = 0, parallelTotal = 0;
        const int trials = 10;
        for (int trial = 0; trial < trials; ++trial) {
            std::vector<int> a = original, b = original;
            std::vector<int> tmpA(n), tmpB(n);
            auto start = std::chrono::steady_clock::now();
            mergeSort(a, tmpA, 0, n);
            auto end = std::chrono::steady_clock::now();
            singleTotal += std::chrono::duration<double, std::milli>(end - start).count();
            start = std::chrono::steady_clock::now();
            parallelMergeSort(b, tmpB, 0, n, threads);
            end = std::chrono::steady_clock::now();
            parallelTotal += std::chrono::duration<double, std::milli>(end - start).count();
            if (!(std::is_sorted(a.begin(), a.end()) && a == b)) {
                std::cout << "mergesort is not correct";
                return 0;
            }
        }
        double single = singleTotal / trials;
        double parallel = parallelTotal / trials;
        std::cout << "n = " << n << ", single = " << single
                  << " ms, parallel = " << parallel
                  << " ms, speedup = " << single / parallel << '\n';
    }
}
