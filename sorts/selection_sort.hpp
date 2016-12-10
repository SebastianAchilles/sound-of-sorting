#pragma once
#include <utility>
#include <iterator>

namespace SelectionSort {

    template <typename T>
    void sort(typename T::iterator begin, typename T::iterator end,
              callback_type<T> callback)
    {
        typename T::iterator i, j, min;
        for (i = begin; i < std::prev(end); ++i) {
            min = i;
            for (j = std::next(i); j < end; ++j) {
                callback(*min, *j);
                if (*j < *min) {
                    min = j;
                }
            }
            std::swap(*min, *i);
            callback(*min, *i);
        }
    }

}
