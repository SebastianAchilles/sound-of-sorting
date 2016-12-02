#include <iostream>
#include <string>
#include <random>
#include <cmath>

#include <tclap/CmdLine.h>

#include "sorts.hpp"

using namespace TCLAP;

int main(int argc, const char *argv[]) {
    // fixed parameters
    const size_t N = 25;
    std::vector<double> v(N);

    // parse command line arguments
    std::string waveform;
    try {
        CmdLine cmd("Apply sorting algorithms to a waveform and listen to it "
                    "as it sorts", ' ', "0.1");
        std::vector<std::string> waveforms;
        waveforms.push_back("white-noise");
        waveforms.push_back("sine");
        waveforms.push_back("falling-edge");
        ValuesConstraint<std::string> allowed_waveforms(waveforms);
        ValueArg<std::string> waveform_arg("w", "waveform",
                                           "Waveform prior to sorting", false,
                                           "sine", &allowed_waveforms);
        cmd.add(waveform_arg);
        cmd.parse(argc, argv);
        waveform = waveform_arg.getValue();
    } catch (ArgException& e) {
        std::cerr << e.what() << std::endl;
    }

    if (waveform == "white-noise") {
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<double> uniform;
        for (double& x : v) {
            x = uniform(rng);
        }
    } else if (waveform == "sine") {
        for (size_t i = 0; i < N; ++i) {
            v[i] = sin(2 * M_PI * i / N);
        }
    }

    bubble_sort(v, [&v]() {
            for (const double& x : v) {
                std::cout << x << std::endl;
            }
        });

    return 0;
}
