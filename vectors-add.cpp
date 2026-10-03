#include <immintrin.h>
#include <chrono>
#include <iostream>

const size_t N = 1'000'000;

// 1. наивная версия
void add_naive(const float* a, const float* b, float* c)
{
    for (size_t i = 0; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
}

// 2. SSE
void add_sse(const float* a, const float* b, float* c)
{
    size_t i = 0;

    for (; i + 4 <= N; i += 4) {
        __m128 va = _mm_loadu_ps(a + i);
        __m128 vb = _mm_loadu_ps(b + i);

        __m128 vc = _mm_add_ps(va, vb);

        _mm_storeu_ps(c + i, vc);
    }

    for (; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
}

// 3. AVX
void add_avx(const float* a, const float* b, float* c)
{
    size_t i = 0;

    for (; i + 8 <= N; i += 8) {
        __m256 va = _mm256_loadu_ps(a + i);
        __m256 vb = _mm256_loadu_ps(b + i);

        __m256 vc = _mm256_add_ps(va, vb);

        _mm256_storeu_ps(c + i, vc);
    }

    for (; i < N; ++i) {
        c[i] = a[i] + b[i];
    }
}

int main()
{
    float* a = new float[N];
    float* b = new float[N];
    float* c = new float[N];

    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = N - i;
    }

    auto start = std::chrono::high_resolution_clock::now();

    add_naive(a, b, c);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Naive: "
              << std::chrono::duration<double, std::micro>(end - start).count()
              << "\n";


    start = std::chrono::high_resolution_clock::now();

    add_sse(a, b, c);

    end = std::chrono::high_resolution_clock::now();

    std::cout << "SSE: "
              << std::chrono::duration<double, std::micro>(end - start).count()
              << "\n";


    start = std::chrono::high_resolution_clock::now();

    add_avx(a, b, c);

    end = std::chrono::high_resolution_clock::now();

    std::cout << "AVX: "
              << std::chrono::duration<double, std::micro>(end - start).count()
              << "\n";
    std::cout << "Check: " << c[123] << '\n';

    delete[] a;
    delete[] b;
    delete[] c;
}