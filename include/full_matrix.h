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

    template < typename Other >
    full_matrix(const matrix_expression<T, Other>& other) noexcept : _grid(other.height(), row(other.width(), {}))  {
        this->copy_from(other);
    }

    full_matrix(size_t h, size_t w, T def = {}) : _grid(h, row(w, def)) {}

    template < size_t H, size_t W >
    explicit full_matrix(const std::array<std::array<T, W>, H>& arr) : _grid(H, row(W, {})) {
        static_assert(H > 0 && W > 0, "Arrays for matrix initialization must be-non empty");
        init(arr, H, W);
    }

    explicit full_matrix(const std::valarray<std::valarray<T>>& arr) {
        if(arr.size() > 0) {
            init(arr, arr.size(), arr[0].size());
        }
    }

    full_matrix(const std::initializer_list<vector<T>> list) : _grid(list) {
    }

    T get(size_t row, size_t col) const override {
        return _grid[row][col];
    }

    void set(size_t row, size_t col, const T& val) override {
        _grid[row][col] = val;
    }

    size_t height() const override {
        return _grid.size();
    }

    size_t width() const override {
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
        _grid.resize(height);
        for (auto i = 0; i < height; ++i) {
            _grid[i].resize(width);
            for (auto j = 0; j < width; ++j) {
                (*this)[i][j] = container[i][j];
            }
        }
    }
};
