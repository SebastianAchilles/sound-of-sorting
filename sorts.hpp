#pragma once
#include <functional>

template <typename T>
using callback_type = std::function<void(typename T::value_type,
                                          typename T::value_type)>;

template <typename T>
using sort_algorithm_type =  std::function<void(T&, callback_type<T>)>;

#include "sorts/bubble_sort.hpp"

