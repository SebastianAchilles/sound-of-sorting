#pragma once
#include <functional>
#include <map>
#include <string>

template <typename T>
using callback_type = std::function<void(typename T::value_type,
                                          typename T::value_type)>;

template <typename T>
using sort_algorithm_type =  std::function<void(T&, callback_type<T>)>;

#include "sorts/bubble_sort.hpp"
// include more sort algorithms here!

template <typename T>
std::map<std::string, sort_algorithm_type<T> > algorithms() {
    return std::map<std::string, sort_algorithm_type<T> > {
        {"bubble", BubbleSort::sort<T>},
    };
}

