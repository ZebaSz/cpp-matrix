#ifndef METNUM_TP2_VECTORS_H
#define METNUM_TP2_VECTORS_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <numeric>

using std::vector;

namespace vectors {

    template<typename T, typename S>
    vector<S> convert(const vector<T>& orig) {
        return vector<S>(orig.begin(), orig.end());
    }

    template<typename T>
    vector<T> subtract(const vector<T> &a, const vector<T> &b) {
        assert(a.size() == b.size());
        vector<T> c;
        std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(c), std::minus<T>());
        return c;
    }

    template<typename T>
    T inner_product(const vector<T> &a, const vector<T> &b) {
        assert(a.size() == b.size());
        return std::inner_product(a.begin(), a.end(), b.begin(), 0);
    }

    template<typename T>
    double twoNormSquared(const vector<T> &v) {
        return inner_product(v, v);
    }

    template<typename T>
    double twoNorm(const vector<T> &v) {
        return std::sqrt(twoNormSquared(v));
    }
}

#endif //METNUM_TP2_VECTORS_H
