#pragma once
#include <utility>
#include <iterator>

namespace BubbleSort {

    template <typename T>
    void sort(typename T::iterator begin, typename T::iterator end,
              callback_type<T> callback)
    {
        typename T::iterator it1, it2, it_end;
        if (begin >= std::prev(end))
            return;
        for (it_end = end; it_end != begin; --it_end) {
            for (it2 = std::next(it1 = begin); it2 != it_end; ++it1, ++it2) {
                if (*it1 > *it2) {
                    callback(*it1, *it2);
                    std::swap(*it1, *it2);
                }
            }
        }
    }

}
