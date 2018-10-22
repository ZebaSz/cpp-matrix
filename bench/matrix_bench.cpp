#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <benchmark/benchmark.h>
#include <full_matrix.h>


static void BM_MatrixCreation(benchmark::State& state) {
    for(auto _ : state)
        full_matrix<int> {5, 5};
}
BENCHMARK(BM_MatrixCreation);

static void BM_MatrixDotProduct(benchmark::State& state) {
    full_matrix<int> a = {
            {2, 0, 1},
            {3, 0, 0},
            {5, 1, 1}
    };

    full_matrix<int> b= {
            {1, 0, 1},
            {1, 2, 1},
            {1, 1, 0}
    };


    full_matrix<int> c = {
            {2, 5, 1},
            {4, 3, 1}
    };

    full_matrix<int> d = {
            {1, 0, 0},
            {0, 2, 0},
            {2, 3, 1}
    };

    for(auto _ : state) {

        full_matrix<int> ab = a.dotProduct(b);
        full_matrix<int> cd = c.dotProduct(d);
        full_matrix<int> abd = a.dotProduct(b).dotProduct(d);
    }
}
BENCHMARK(BM_MatrixDotProduct);

static void BM_MatrixTranspose(benchmark::State& state) {
    constexpr int m_h = 5;
    int value = 1;
    std::array<std::array<int, m_h>, m_h> arr{};
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            arr[i][j] = value;
            value++;
        }
    }
    full_matrix<int> m(arr);

    constexpr size_t sizeN = 5;
    constexpr size_t sizeM = 6;
    int anotherValue = 1;
    std::valarray<std::valarray<int>> notSquared_arr{{0, sizeN}, sizeM};
    for (int i = 0; i < sizeM; ++i) {
        for (int j = 0; j < sizeN; ++j) {
            notSquared_arr[i][j] = anotherValue;
            anotherValue++;
        }
    }
    full_matrix<int> notSquared_m(notSquared_arr);

    for (auto _ : state) {
        full_matrix<int> bench1 = m.transpose();
        full_matrix<int> bench2 = m.transpose().transpose();

        full_matrix<int> bench3 = notSquared_m.transpose();
    }

}
BENCHMARK(BM_MatrixTranspose);

BENCHMARK_MAIN();

#pragma clang diagnostic pop