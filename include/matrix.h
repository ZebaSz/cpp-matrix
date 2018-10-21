#pragma once

#include <cstddef>
#include <cmath>
#include <memory>
#include <type_traits>
#include "vectors.h"

template < typename T, typename Derived >
class matrix {
public:

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;

    class iterator;
    class const_iterator;

    class wrapper {
    public:

        operator value_type() const { // NOLINT(google-explicit-constructor)
            return _matrix->get(_r, _c);
        }

        wrapper& operator=(value_type val) {
            _matrix->set(_r, _c, val);
            return *this;
        }

        explicit wrapper(matrix* mx, size_t r, size_t c) : _matrix(mx), _r(r), _c(c) {}

    protected:
        matrix* _matrix;
        size_t _r;
        size_t _c;
    };

    template < bool is_const >
    class base_row {
    public:
        using matrix_type = typename std::conditional< is_const, const matrix*, matrix *>::type;
        using iterator_type = typename std::conditional< is_const, const_iterator, iterator >::type;

        size_t size() const {
            return _matrix->width();
        }

        explicit operator vector<T>() const {
            vector<T> v(size(), static_cast<T>(0));
            for (size_t i = 0; i < size(); ++i) {
                v[i] = _matrix->get(_row, i);
            }
            return v;
        }

        iterator_type begin() {
            return iterator_type(_matrix, _row);
        }

        iterator_type end() {
            return iterator_type(_matrix, _row + 1);
        }

        bool operator==(const base_row& other) const {
            return _matrix == other._matrix && _row == other._row;
        }

    protected:
        base_row(matrix_type mx, size_t r) : _matrix(mx), _row(r) {}

        matrix_type _matrix;
        size_t _row;
    };

    class row : public base_row<false> {
    public:
        explicit row(matrix* m, size_t r = 0) : base_row<false>(m, r) {}

        wrapper operator[](size_t pos) {
            return wrapper(this->_matrix, this->_row, pos);
        }

    };
    class const_row : public base_row<true> {
    public:
        explicit const_row(const matrix* m, size_t r = 0) : base_row<true>(m, r) {}

        value_type operator[](size_t pos) {
            return this->_matrix->get(this->_row, pos);
        }

    };

    template < typename IterDerived, bool is_const >
    class base_iterator : public std::iterator<std::input_iterator_tag, value_type > {
    public:
        using matrix_type = typename std::conditional< is_const, const matrix*, matrix *>::type;
        using ref_type = typename std::conditional< is_const, value_type, wrapper >::type;

        IterDerived& operator++() {
            if(++_col >= _matrix->width()) {
                _col = 0;
                ++_row;
            }
            return static_cast<IterDerived&>(*this);
        }

        const IterDerived operator++(int) {
            IterDerived orig(*this);
            operator++();
            return orig;
        }

        bool operator==(const IterDerived& other) const {
            return _row == other._row && _col == other._col;
        }

        bool operator!=(const IterDerived& other) const {
            return ! operator==(other);
        }

        ref_type operator*() {
            return (*_matrix)[_row][_col];
        }

    protected:
        base_iterator() = default;
        explicit base_iterator(matrix_type m, size_t r = 0, size_t c = 0) : _matrix(m), _row(r), _col(c) {}

        matrix_type _matrix = nullptr;
        size_t _row = 0;
        size_t _col = 0;
    };

    class iterator : public base_iterator<iterator, false> {
    public:
        iterator() = default;
        explicit iterator(matrix* m, size_t r = 0, size_t c = 0) : base_iterator<iterator, false>(m, r, c) {}
    };
    class const_iterator : public base_iterator<const_iterator, true> {
    public:
        const_iterator() = default;
        explicit const_iterator(const matrix* m, size_t r = 0, size_t c = 0) : base_iterator<const_iterator, true>(m, r, c) {}
    };

    template < bool is_const >
    class base_row_list {
    public:
        using matrix_type = typename std::conditional< is_const, const matrix*, matrix *>::type;
        using row_type = typename std::conditional< is_const, const_row, row>::type;

        class iterator : public std::iterator<std::input_iterator_tag, row_type> {
        public:
            using matrix_type = typename std::conditional< is_const, const matrix, matrix>::type;

            iterator& operator++() {
                ++_row;
                return *this;
            }

            const iterator operator++(int) {
                iterator orig(*this);
                operator++();
                return orig;
            }

            bool operator==(const iterator& other) {
                return _matrix == other._matrix &&  _row == other._row;
            }

            bool operator!=(const iterator& other) {
                return ! operator==(other);
            }

            row_type operator*() {
                return row_type(_matrix, _row);
            }
        protected:
            explicit iterator(matrix_type* m, size_t r = 0) : _matrix(m), _row(r) {}
            iterator(const iterator&) = default;

            matrix_type* const _matrix;
            size_t _row;
        };

    protected:
        explicit base_row_list(matrix_type m) : _matrix(m) {};

        matrix_type _matrix;
    };

    class row_list : base_row_list<false> {
    public:
        explicit row_list(matrix* m) : base_row_list<false>(m) {}
    };

    class const_row_list : base_row_list<true> {
    public:
        explicit const_row_list(const matrix* m) : base_row_list<true>(m) {}
    };

    virtual ~matrix() = default;

    row operator[](size_t pos) {
        return row(this, pos);
    }

    const_row operator[](size_t pos) const {
        return const_row(this, pos);
    }

    // common operations

    value_type get(size_t row, size_t col) const {
        if(transposed) {
            return internal_get(col, row);
        } else {
            return internal_get(row, col);
        }
    }

    void set(size_t row, size_t col, const_reference val) {
        if(transposed) {
            internal_set(col, row, val);
        } else {
            internal_set(row, col, val);
        }
    }

    size_t width() const {
        if(transposed) {
            return internal_height();
        } else {
            return internal_width();
        }
    }

    size_t height() const {
        if(transposed) {
            return internal_width();
        } else {
            return internal_height();
        }
    }

    iterator begin() {
        return iterator(this);
    }

    const_iterator begin() const {
        return const_iterator(this);
    }

    iterator end() {
        return iterator(this, height(), width());
    }

    const_iterator end() const {
        return const_iterator(this, height(), width());
    }

    row_list rows() {
        return row_list(this);
    }

    const_row_list rows() const {
        return const_row_list(this);
    }

    Derived dotProduct(const_reference a) const {
        Derived result(height(), width());

        for (size_t i = 0; i < height(); i++)
            for (size_t j = 0; j < width(); j++)
                result[i][j] = get(i,j) * a;

        return result;
    }

    Derived dotProduct(const matrix &b) const {
        Derived c(height(), b.width());

        // matrices must be non-empty
        assert(height() > 0 && width() > 0);
        assert(b.height() > 0 && b.width() > 0);
        // input dimensions must match
        assert(width() == b.height());
        // output dimensions must match
        assert(c.height() == height());
        assert(c.width() == b.width());

        for (size_t i = 0; i < c.height(); ++i) {
            for (size_t j = 0; j < c.width(); ++j) {
                T val = 0;
                for (size_t k = 0; k < b.height(); ++k) {
                     val += get(i,k) * b.get(k, j);
                }
                c[i][j] = val;
            }
        }
        return c;
    }

    vector<T> dotProduct(const vector<T> &b) const {
        vector<T> c(height(), 0);
        // matrices must be non-empty
        assert(height() > 0 && width() > 0);
        assert(b.size() > 0);
        // input dimensions must match
        assert(width() == b.size());
        // output dimensions must match
        assert(c.size() == height());

        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                c[i] += get(i,j) * b[j];
            }
        }
        return c;
    }

    template < typename Other >
    Derived operator+(const matrix<T, Other> &b) const {
        Derived c(height(), width());

        // matrices must be non-empty
        assert(height() > 0 && width() > 0);
        // input dimensions must match
        assert(height() == b.height());
        assert(width() == b.width());
        // output dimensions must match
        assert(height() == c.height());
        assert(width() == c.width());

        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                c[i][j] = get(i,j) + b.get(i,j);
            }
        }
        return c;
    }

    template < typename Other >
    Derived operator-(const matrix<T, Other> &b) const {
        Derived c(height(), width());

        // matrices must be non-empty
        assert(height() > 0 && width() > 0);
        // input dimensions must match
        assert(height() == b.height());
        assert(width() == b.width());
        // output dimensions must match
        assert(height() == c.height());
        assert(width() == c.width());

        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                c[i][j] = get(i,j) - b.get(i,j);
            }
        }
        return c;
    }

    Derived operator*(const_reference a) const {
        Derived result(*this);

        for (auto r : result)
            for (auto c : r)
                c *= a;

        return result;
    }

    void inplaceTranspose() {
        transposed = !transposed;
    }

    Derived transpose() const {
        Derived t(width(), height());
        // matrix must be non-empty
        assert(height() > 0 && width() > 0);
        // output dimensions must match
        assert(height() == t.width());
        assert(width() == t.height());

        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                t[j][i] = get(i,j);
            }
        }
        return t;
    }

    Derived transposedProduct() const {
        Derived t(height(), height());
        // matrix must be non-empty
        assert(height() > 0 && width() > 0);
        // output dimensions must match
        assert(height() == t.width());
        assert(height() == t.height());

        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j <= i; ++j) {
                auto val = static_cast<T>(0);
                for (size_t k = 0; k < width(); ++k) {
                    val += get(i,k) * get(j,k);
                }
                t[i][j] = val;
                if (i != j) {
                    t[j][i] = val;
                }
            }
        }
        return t;
    }

    double infinityNorm() const {
        assert(height() > 0 && width() > 0);
        double result = get(0,0);
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                if (result < get(i,j)) {
                    result = get(i,j);
                }
            }
        }
        return result;
    }

    double twoNorm() const {
        double result = 0;
        for (size_t i = 0; i < height(); ++i) {
            double c = 0.0;
            for (size_t j = 0; j < width(); ++j) {
                // Super Kahan2!!
                double y = std::pow(get(i,j), 2) - c;
                double t = result + y;
                c = (t - result) - y;
                result = t;
            }
        }
        return sqrt(result);
    }

    double singleNorm() const {
        double result = 0;
        for (size_t i = 0; i < height(); ++i) {
            double c = 0.0;
            for (size_t j = 0; j < width(); ++j) {
                // Super Kahan!!
                double y = double(get(i,j)) - c;
                double t = result + y;
                c = (t - result) - y;
                result = t;
            }
        }
        return result;
    }

    bool isSquared() const {
        return height() == width();
    }

    bool isDiagonal() const {
        if(!isSquared()) {
            return false;
        }
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < width(); ++j) {
                if (i != j && get(i,j) != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isLowerTriangular() const {
        if(!isSquared()) {
            return false;
        }
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = i + 1; j < width(); ++j) {
                if (get(i,j) != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isUpperTriangular() const {
        if(!isSquared()) {
            return false;
        }
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (get(i,j) != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool isSymmetric() const {
        if(!isSquared()) {
            return false;
        }
        for (size_t i = 0; i < height(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (get(i,j) != get(j,i)) {
                    return false;
                }
            }
        }
        return true;
    }

    template < typename Other >
    bool operator==(const matrix<T, Other>& other) const {
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

protected:
    matrix() noexcept = default;

    template < typename Other >
    explicit matrix(const matrix<T, Other>& other) noexcept {
        for (int i = 0; i < other.height(); ++i) {
            for (int j = 0; j < width(); ++j) {
                (*this)[i][j] = other[i][j];
            }
        }
    }

    // subclass-defined core functions
    virtual value_type internal_get(size_t row, size_t col) const = 0;

    virtual void internal_set(size_t row, size_t col, const_reference val) = 0;

    virtual size_t internal_width() const = 0;

    virtual size_t internal_height() const = 0;

private:
    bool transposed = false;
};
