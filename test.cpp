#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

#include "sorts.hpp"

int main () {
    const size_t N = 1000;
    std::vector<int> v(N);

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> uniform(0, N);

    for (auto& e : v)
        e = uniform(rng);

    std::vector<int> sorted = v;
    std::sort(sorted.begin(), sorted.end());

    auto map = algorithms<decltype(v)>();
    auto dummy = [](int i, int j) {};

    for (auto& p : map) {
        std::vector<int> w = v;
        std::cout << "Testing " << p.first << " sort...\t" << std::flush;
        (p.second)(w, dummy);
        if (w == sorted) {
            std::cout << "ok." << std::endl;
        } else {
            std::cout << "FAILED!" << std::endl;
        }
    }

    return 0;
}
