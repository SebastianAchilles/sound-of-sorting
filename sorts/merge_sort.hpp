#pragma once
#include <utility>
#include <iterator>

namespace MergeSort {

    namespace detail {

        template <typename T>
        void merge(typename T::iterator itl, typename T::iterator m,
                   typename T::iterator itr, callback_type<T> callback)
        {
            T b(itr - itl);
            typename T::iterator i = b.begin();
            typename T::iterator j = itl;
            typename T::iterator k = m;
            while (j < m && k < itr) {
                callback(*j, *k);
                if (*j <= *k) {
                    *(i++) = *(j++);
                } else {
                    *(i++) = *(k++);
                }
            }
            while (j < m)
                *(i++) = *(j++);
            while (k < itr)
                *(i++) = *(k++);
            for (j = itl, i = b.begin(); j < itr; ++j, ++i)
                *j = *i;
        }
    }

    template <typename T>
    void sort(typename T::iterator itl, typename T::iterator itr,
              callback_type<T> callback)
    {
        if (itl < std::prev(itr)) {
            typename T::iterator m = itl + (itr - itl) / 2;
            sort<T>(itl, m, callback);
            sort<T>(m, itr, callback);
            detail::merge<T>(itl, m, itr, callback);
        }
    }

}
