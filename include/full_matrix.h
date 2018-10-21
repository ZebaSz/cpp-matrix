#pragma once

#include <vector>
#include <valarray>
#include "matrix.h"

template < typename T >
class full_matrix : public matrix<T, full_matrix<T>> {
public:

    using row = std::vector<T>;

    using grid = std::vector< row >;

    full_matrix(const full_matrix&) noexcept = default;

    full_matrix(full_matrix&&) noexcept = default;

    template < typename Derived >
    explicit full_matrix(const matrix<T, Derived>& other) noexcept : matrix<T, full_matrix<T>>(other) {}

    full_matrix(size_t h, size_t w, T def = static_cast<T>(0)) : _grid(h, row(w, def)) {}

    template < size_t H, size_t W >
    explicit full_matrix(const std::array<std::array<T, W>, H>& arr) : _grid(H, row(W, static_cast<T>(0))) {
        static_assert(H > 0 && W > 0, "Arrays for matrix initialization must be-non empty");
        init(arr, H, W);
    }

    explicit full_matrix(const std::valarray<std::valarray<T>>& arr) {
        if(arr.size() > 0) {
            init(arr, arr.size(), arr[0].size());
        }
    }

    template <typename Iterable >
    explicit full_matrix(const Iterable& other) : _grid(other) {}

    full_matrix(const std::initializer_list<vector<T>> list) : _grid(list) {
    }

    T internal_get(size_t row, size_t col) const override {
        return _grid[row][col];
    }

    void internal_set(size_t row, size_t col, const T& val) override {
        _grid[row][col] = val;
    }

    size_t internal_height() const override {
        return _grid.size();
    }

    size_t internal_width() const override {
        if(_grid.empty()) {
            return 0;
        }
        return _grid[0].size();
    }

    // special generators

    static full_matrix zero(size_t size) {
        return full_matrix(size, size);
    }

    static full_matrix identity(size_t size) {
        full_matrix mx = zero(size);
        for (size_t i = 0; i < size; ++i) {
            mx[i][i] = 1;
        }
        return mx;
    }

private:
    grid _grid;

    template < typename Iterable >
    void init(const Iterable& container, size_t height, size_t width) {
        _grid.reserve(height);
        for (auto i = 0; i < height; ++i) {
            _grid[i].reserve(width);
            for (auto j = 0; j < width; ++j) {
                (*this)[i][j] = container[i][j];
            }
        }
    }

};
