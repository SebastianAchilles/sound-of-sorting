#pragma once
#include <functional>
#include <map>
#include <string>

template <typename T>
using callback_type = std::function<void(typename T::value_type,
                                         typename T::value_type)>;

template <typename T>
using sort_algorithm_type = std::function<void(typename T::iterator,
                                               typename T::iterator,
                                               callback_type<T>)>;

#include "sorts/bubble_sort.hpp"
#include "sorts/quick_sort.hpp"
/* include more sort algorithms here! */

template <typename T>
std::map<std::string, sort_algorithm_type<T> > algorithms() {
    return std::map<std::string, sort_algorithm_type<T> > {
        {"bubble", BubbleSort::sort<T>},
        {"quick", QuickSort::sort<T>},
        /* include more sort algorithms here! */
    };
}

