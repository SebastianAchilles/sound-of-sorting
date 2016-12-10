#pragma once
#include <utility>

namespace BubbleSort {

    template <typename T>
    void sort(T& v, callback_type<T> callback)
    {
        typename T::iterator it1, it2, it_end;
        if (v.size() <= 1)
            return;
        for (it_end = v.end(); it_end != v.begin(); --it_end) {
            for (it2 = std::next(it1 = v.begin()); it2 != it_end; ++it1, ++it2) {
                if (*it1 > *it2) {
                    callback(*it1, *it2);
                    std::swap(*it1, *it2);
                }
            }
        }
    }

}
