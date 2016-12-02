#include <iostream>
#include <random>

#include "sorts.hpp"

int main(int argc, const char *argv[]) {
    const size_t N = 100;
    std::vector<double> v(N);

    {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<double> uniform;
        for (double& x : v) {
            x = uniform(rng);
        }
    }

    bubble_sort(v, [&v]() {
            for (double &x : v) {
                std::cout << x << std::endl;
            }
        });

    return 0;
}
