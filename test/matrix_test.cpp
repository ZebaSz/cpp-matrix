#include <gtest/gtest.h>
#include "full_matrix.h"
#include "sparse_matrix.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

constexpr int m_h = 5;

TEST(matrix_test, fullIdentity) {
    full_matrix<int> m (m_h, m_h, 0);
    for (size_t i = 0; i < m_h; ++i) {
        m[i][i] = 1;
    }
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            if(i != j) {
                ASSERT_EQ(m[i][j], 0);
            } else {
                ASSERT_EQ(m[i][j], 1);
            }
        }
    }
}

TEST(matrix_test, sparseIdentity) {
    sparse_matrix<int> m (m_h, m_h, 0);
    for (size_t i = 0; i < m_h; ++i) {
        m[i][i] = 1;
    }
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            if(i != j) {
                ASSERT_EQ(m[i][j], 0);
            } else {
                ASSERT_EQ(m[i][j], 1);
            }
        }
    }
}

TEST(matrix_test, square_array) {
    constexpr int value = 1;
    std::array<int, m_h> arr_row{};
    arr_row.fill(value);
    std::array<std::array<int, m_h>, m_h> arr {};
    arr.fill(arr_row);
    full_matrix<int> m (arr);
    ASSERT_EQ(m.height(), m_h);
    for (size_t i = 0; i < m.height(); ++i) {
        ASSERT_EQ(m[i].size(), m_h);
        for (size_t j = 0; j < m.width(); ++j) {
            ASSERT_EQ(m[i][j], value);
        }
    }
}


TEST(matrix_test, dotProduct) {
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

    full_matrix<int> expected = {
            {3, 1, 2},
            {3, 0, 3},
            {7, 3, 6}
    };

    auto result = a.dotProduct(b);

    ASSERT_EQ(result, expected);

    full_matrix<int> i3 = full_matrix<int>::identity(3);

    ASSERT_EQ(a.dotProduct(i3), a);
    ASSERT_EQ(b.dotProduct(i3), b);
    ASSERT_EQ(i3.dotProduct(a), a);
    ASSERT_EQ(i3.dotProduct(b), b);

    full_matrix<int> c = {
            {2, 5, 1},
            {4, 3, 1}
    };

    full_matrix<int> d = {
            {1, 0, 0},
            {0, 2, 0},
            {2, 3, 1}
    };

    full_matrix<int> ex2 = {
            {4, 13, 1},
            {6,  9, 1}
    };

    ASSERT_EQ(c.dotProduct(d), ex2);
}


TEST(matrix_test, sum) {
    full_matrix<int> a = {
            {2, 0, 1},
            {3, 0, 0},
            {5, 1, 1}
    };

    full_matrix<int> b = {
            {1, 0, 1},
            {1, 2, 1},
            {1, 1, 0}
    };

    full_matrix<int> expected = {
            {3, 0, 2},
            {4, 2, 1},
            {6, 2, 1}
    };

    auto result = a + b;

    ASSERT_EQ(result, expected);
}

TEST(matrix_test, norm) {
    int value = 1;
    full_matrix<int> m (m_h, m_h, value);
    ASSERT_EQ(m.infinityNorm(), 1);
    ASSERT_EQ(m.twoNorm(), 5);
    ASSERT_EQ(m.singleNorm(), 25);
}

TEST(matrix_test, traspose){
    int value = 1;
    std::array<std::array<int, m_h>, m_h> arr {};
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            arr[i][j] = value;
            value++;
        }
    }
    full_matrix<int> m (arr);
    auto transposedM = m.transpose();
    auto transposed_transposedM = transposedM.transpose();

    ASSERT_EQ(m, transposed_transposedM);

    constexpr size_t sizeN = 5;
    constexpr size_t sizeM = 6;
    int anotherValue = 1;
    std::valarray<std::valarray<int>> notSquared_arr{{0, sizeN},sizeM};
    for (int i = 0; i < sizeM; ++i) {
        for (int j = 0; j < sizeN; ++j) {
            notSquared_arr[i][j] = anotherValue;
            anotherValue++;
        }
    }
    full_matrix<int> notSquared_m(notSquared_arr);
    auto notSquared_transposedM = notSquared_m.transpose();
    auto notSquared_transposed_transposedM = notSquared_transposedM.transpose();

    ASSERT_EQ(notSquared_m, notSquared_transposed_transposedM);
}

TEST(matrix_test, inplaceTranspose) {
    full_matrix<int> m (m_h, m_h);

    int value = 1;
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            m[i][j] = value;
            value++;
        }
    }

    auto m_t = m.transpose();
    m.inplaceTranspose();
    ASSERT_EQ(m, m_t);
}

TEST(matrix_test, isLowerTriangular) {
    int value = 1;
    std::array<std::array<int, m_h>, m_h> arr {};
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            if (j < i+1) {
                arr[i][j] = value;
                value++;
            } else {
                arr[i][j] = 0;
            }
        }
    }
    full_matrix<int> m (arr);
    ASSERT_TRUE(m.isLowerTriangular());
}

TEST(matrix_test, isUpperTriangular) {
    int value = 1;
    std::array<std::array<int, m_h>, m_h> arr {};
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            if (j >= i) {
                arr[i][j] = value;
                value++;
            } else {
                arr[i][j] = 0;
            }
        }
    }
    full_matrix<int> m (arr);
    ASSERT_TRUE(m.isUpperTriangular());
}

TEST(matrix_test, trasposeProduct) {
    int value = 1;
    std::array<std::array<int, m_h>, m_h> arr {};
    for (size_t i = 0; i < m_h; ++i) {
        for (size_t j = 0; j < m_h; ++j) {
            arr[i][j] = value;
            value++;
        }
    }

    full_matrix<int> m (arr);
    ASSERT_EQ(m.dotProduct(m.transpose()), m.transposedProduct());
}

#pragma clang diagnostic pop