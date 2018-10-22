#pragma once
#include <cstddef>

#include "matrix_expr.h"

using std::vector;

template < typename T, typename M1, typename M2 >
struct matrix_plus {

    T operator()(const M1& a, const M2& b, size_t row, size_t col) const {
        return a.get(row, col) + b.get(row, col);
    }

    void assert_sizes(const M1& a, const M2& b) const {
        assert(a.height() == b.height());
        assert(a.width() == b.width());
    }
};

template < typename T, typename M1, typename M2 >
struct matrix_minus {

    T operator()(const M1& a, const M2& b, size_t row, size_t col) const {
        return a.get(row, col) - b.get(row, col);
    }

    void assert_sizes(const M1& a, const M2& b) const {
        assert(a.height() == b.height());
        assert(a.width() == b.width());
    }
};

template < typename T, typename M, typename S >
struct matrix_scalar_times {

    T operator()(const M& a, const S& b, size_t row, size_t col) const {
        return a.get(row, col) * b;
    }
};

template < typename T, typename M, typename S >
struct matrix_vector_product_op {

    T operator()(const M& a, const vector<S>& b, size_t row, size_t col) const {
        T val {0};
        for (size_t k = 0; k < b.height(); ++k) {
            val += a.get(row,k) * b.get(k, col);
        }
        return val;
    }

    void assert_sizes(const M& a, const vector<S>& b) const {
        assert(a.width() = b.size());
    }
};

template < typename T, typename M1, typename M2 >
struct matrix_dot_product_op {

    T operator()(const M1& a, const M2& b, size_t row, size_t col) const {
        T val {0};
        for (size_t k = 0; k < b.height(); ++k) {
            val += a.get(row,k) * b.get(k, col);
        }
        return val;
    }

    void assert_sizes(const M1& a, const M2& b) const {
        // matrices must be non-empty
        assert(a.height() > 0 && a.width() > 0);
        assert(b.height() > 0 && b.width() > 0);
        // input dimensions must match
        assert(a.width() == b.height());
    }
};


template < typename T, typename M >
struct matrix_transpose_op {

    T operator()(const M &a, size_t row, size_t col) const {
        return a.get(col, row);
    }
};