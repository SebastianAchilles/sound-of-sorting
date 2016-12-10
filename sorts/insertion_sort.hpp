#pragma once
#include <utility>
#include <iterator>

namespace InsertionSort {

    template <typename T>
    void sort(typename T::iterator begin, typename T::iterator end,
              callback_type<T> callback)
    {
        typename T::iterator i, j;
        for (i = begin; i < end; ++i) {
            for (j = i; j > begin && *std::prev(j) > *j; --j) {
                std::swap(*std::prev(j), *j);
                callback(*std::prev(j), *j);
            }
        }
    }

}
