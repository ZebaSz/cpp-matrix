#pragma once

#include <cstddef>
#include <map>
#include "matrix.h"

using coord = std::pair<size_t, size_t>;

template < typename T >
class sparse_matrix : public matrix<T, sparse_matrix<T>> {
public:

    sparse_matrix(const sparse_matrix&) noexcept = default;

    sparse_matrix(sparse_matrix&&) noexcept = default;

    sparse_matrix(size_t height, size_t width, const T& def = static_cast<T>(0)) : h(height), w(width), _def(def) {}

    T internal_get(size_t row, size_t col) const override {
        auto it = grid.find(coord(row, col));
        if(it == grid.end()) {
            return _def;
        }
        return it->second;
    }

    void internal_set(size_t row, size_t col, const T& val) override {
        coord key(row, col);
        if(val == static_cast<T>(0)) {
            grid.erase(key);
        } else {
            grid[key] = val;
        }
    }

    size_t internal_height() const override {
        return h;
    }

    size_t internal_width() const override {
        return w;
    }

    // special generators

    static sparse_matrix zero(size_t size) {
        return sparse_matrix(size, size, static_cast<T>(0));
    }

    static sparse_matrix identity(size_t size) {
        sparse_matrix mx(size, size, static_cast<T>(0));
        for (size_t i = 0; i < size; ++i) {
            mx[i][i] = 1;
        }
        return mx;
    }

protected:
    std::map< coord, T > grid;

    size_t h;
    size_t w;
    const T _def;
};
