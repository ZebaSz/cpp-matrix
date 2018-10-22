#pragma once

#include "matrix_ops.h"
#include "vectors.h"

template < class T, class Derived >
class matrix_expression;

template < typename T, typename M, typename Op, typename Derived >
class matrix_single_expr : public matrix_expression<T, Derived> {
public:
    explicit matrix_single_expr(const M& m) : _m(m), _op() {}

    T get(size_t row, size_t col) const override {
        return _op(_m, row, col);
    }

protected:
    const M& _m;
    const Op _op;
};

template < typename T, typename M, typename S, typename Op, typename Derived >
class matrix_scalar_expr : public matrix_expression<T, Derived> {
public:
    matrix_scalar_expr(const M& a, const S& b) : _a(a), _b(b), _op() {}

    T get(size_t row, size_t col) const override {
        return _op(_a, _b, row, col);
    }

    size_t height() const override {
        return _a.height();
    }

    size_t width() const override {
        return _a.width();
    }

protected:
    const M& _a;
    const S _b;
    const Op _op;
};

template < typename T, typename M, typename S, typename Op, typename Derived >
class matrix_vector_expr : public matrix_expression<T, Derived> {
public:
    matrix_vector_expr(const M& a, const vector<S>& b) : _a(a), _b(b), _op() {
        _op.assert_sizes(_a, _b);
    }


    T get(size_t row, size_t col) const override {
        return _op(_a, _b, row, col);
    }

    size_t height() const override {
        return _a.height();
    }

    size_t width() const override {
        return 1;
    }

    operator vector<T>() {
        vector<T> res(height());
        for (size_t i = 0; i < height(); ++i) {
            res[i] = get(i, 0);
        }
        return res;
    }

protected:
    const M& _a;
    const vector<S> _b;
    const Op _op;
};

template < typename T, typename M1, typename M2, typename Op, typename Derived >
class matrix_matrix_expr : public matrix_expression<T, Derived> {
public:
    matrix_matrix_expr(const M1& a, const M2& b) : _a(a), _b(b), _op() {
        _op.assert_sizes(_a, _b);
    }

    T get(size_t row, size_t col) const override {
        return _op(_a, _b, row, col);
    }

    size_t height() const override {
        return _a.height();
    }

    size_t width() const override {
        return _a.width();
    }

protected:
    const M1& _a;
    const M2& _b;
    const Op _op;
};

template < typename T, typename M1, typename M2 >
class matrix_sum : public matrix_matrix_expr<
        T, M1, M2,
        matrix_plus<T,M1,M2>,
        matrix_sum<T, M1, M2>> {
    using matrix_matrix_expr<
            T, M1, M2,
            matrix_plus<T,M1,M2>,
            matrix_sum<T, M1, M2>>
    ::matrix_matrix_expr;
};

template < typename T, typename M1, typename M2 >
class matrix_difference : public matrix_matrix_expr<
        T, M1, M2,
        matrix_minus<T,M1,M2>,
        matrix_difference<T, M1, M2>> {
    using matrix_matrix_expr<
            T, M1, M2,
            matrix_minus<T,M1,M2>,
            matrix_difference<T, M1, M2>>
    ::matrix_matrix_expr;
};

template < typename T, typename M, typename S >
class matrix_scalar_product : public matrix_scalar_expr<T, M, S,
        matrix_scalar_times<T,M,S>,
        matrix_scalar_product<T, M, S>> {

    using matrix_scalar_expr<T, M, S,
            matrix_scalar_times<T,M,S>,
            matrix_scalar_product<T,M,S>>
    ::matrix_scalar_expr;
};

template < typename T, typename M, typename S >
class matrix_vector_product : public matrix_vector_expr<
        T, M, S,
        matrix_vector_product_op<T,M,S>,
        matrix_vector_product<T, M, S>> {
    using matrix_vector_expr<
            T, M, S,
            matrix_vector_product_op<T,M,S>,
            matrix_vector_product<T, M, S>>
    ::matrix_vector_expr;

    size_t height() const override {
        return this->_a.height();
    }

    size_t width() const override {
        return this->_b.width();
    }
};

template < typename T, typename M >
class matrix_transpose : public matrix_single_expr<
        T, M,
        matrix_transpose_op<T,M>,
        matrix_transpose<T, M>> {
public:
    using matrix_single_expr<
            T, M,
            matrix_transpose_op<T,M>,
            matrix_transpose<T, M>>
    ::matrix_single_expr;

    size_t height() const override {
        return this->_m.width();
    }

    size_t width() const override {
        return this->_m.height();
    }
};

template < typename T, typename M1, typename M2 >
class matrix_dot_product : public matrix_matrix_expr<
        T, M1, M2,
        matrix_dot_product_op<T,M1,M2>,
        matrix_dot_product<T, M1, M2>> {
public:
    using matrix_matrix_expr<
            T, M1, M2,
            matrix_dot_product_op<T,M1,M2>,
            matrix_dot_product<T, M1, M2>>
    ::matrix_matrix_expr;

    size_t height() const override {
        return this->_a.height();
    }

    size_t width() const override {
        return this->_b.width();
    }
};

template < class T, class Derived >
class matrix_expression {
public:
    template < typename S, typename Other >
    matrix_sum<T, Derived, Other> operator+(const matrix_expression<S, Other> &that) const {
        return matrix_sum<T, Derived, Other>(
                static_cast<const Derived&>(*this),
                static_cast<const Other&>(that));
    }

    template < typename S, typename Other >
    matrix_difference<T, Derived, Other> operator-(const matrix_expression<S, Other> &that) const {
        return matrix_difference<T, Derived, Other>(
                static_cast<const Derived&>(*this),
                static_cast<const Other&>(that));
    }

    template < typename S >
    matrix_scalar_product<T, Derived, S> operator*(const S &that) const {
        return matrix_scalar_product<T, Derived, S>(
                static_cast<const Derived&>(*this),
                that);
    }

    template < typename S, typename Other >
    matrix_dot_product<T, Derived, Other> dotProduct(const matrix_expression<S, Other> &that) const {
        return matrix_dot_product<T, Derived, Other>(
                static_cast<const Derived&>(*this),
                static_cast<const Other&>(that));
    }

    template < typename S >
    matrix_vector_product<T, Derived, S> dotProduct(const vector<S> &that) const {
        return matrix_vector_product<T, Derived, S>(
                static_cast<const Derived&>(*this),
                that);
    }

    matrix_transpose<T, Derived> transpose() const {
        return matrix_transpose<T, Derived>(
                static_cast<const Derived&>(*this));
    }

    template < typename S, typename Other >
    bool operator==(const matrix_expression<S, Other>& other) const {
        if(height() != other.height() || width() != other.width()) {
            return false;
        }
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                if(get(i,j) != other.get(i,j)) {
                    return false;
                }
            }
        }
        return true;
    }

    // basic operations for subclasses

    virtual T get(size_t row, size_t col) const = 0;

    virtual size_t height() const = 0;

    virtual size_t width() const = 0;
};