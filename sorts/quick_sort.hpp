#pragma once
#include <utility>
#include <iterator>

namespace QuickSort {

    namespace detail {

        template <typename T>
        typename T::iterator partition(typename T::iterator itl,
                                       typename T::iterator itr,
                                       callback_type<T> callback)
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

    }

    template <typename T>
    void sort(typename T::iterator itl, typename T::iterator itr,
              callback_type<T> callback)
    {
        if (itl < std::prev(itr)) {
            typename T::iterator p = detail::partition<T>(itl, itr, callback);
            sort<T>(itl, p, callback);
            sort<T>(std::next(p), itr, callback);
        }
    }

}
