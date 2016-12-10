#pragma once
#include <utility>
#include <iterator>

namespace QuickSort {

    namespace detail {

        template <typename T>
        typename T::iterator partition(T& v, callback_type<T> callback,
                                       typename T::iterator itl,
                                       typename T::iterator itr)
        {
            --itr; // pivot element
            typename T::iterator i = itl;
            for (typename T::iterator j = itl; j < itr; ++j) {
                if (*j <= *itr) {
                    std::swap(*i, *j);
                    callback(*i, *j);
                    ++i;
                }
            }
            std::swap(*i, *itr);
            callback(*i, *itr);
            return i;
        }

        template <typename T>
        void quicksort(T& v, callback_type<T> callback,
                       typename T::iterator itl, typename T::iterator itr)
        {
            if (itl < std::prev(itr)) {
                typename T::iterator p = partition(v, callback, itl, itr);
                quicksort(v, callback, itl, p);
                quicksort(v, callback, std::next(p), itr);
            }
        }

    }

    template <typename T>
    void sort(T& v, callback_type<T> callback) {
        detail::quicksort(v, callback, v.begin(), v.end());
    }

}
