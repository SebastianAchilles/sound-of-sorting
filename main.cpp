#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <cstdint>
#include <limits>

#include <tclap/CmdLine.h>
#include <SDL2/SDL.h>

#include "sorts.hpp"
#include "playback.hpp"

using namespace TCLAP;

typedef int16_t sample_t;

static uint8_t *audio_pos;
static size_t audio_len;

void audio_callback(void *, uint8_t *, int);

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        return playback();
    }
    
    // fixed parameters
    const uint32_t sample_rate = 44100;
    const sample_t amplitude = std::numeric_limits<sample_t>::max();
    std::vector<sample_t> v;
    std::vector<sample_t> wav;
    auto algorithm_map = algorithms<decltype(wav)>();

    // parse command line arguments
    std::string input;
    bool waveform_mode;
    sort_algorithm_type<decltype(wav)> sort;
    size_t N;
    try {
        CmdLine cmd("Apply sorting algorithms to a waveform and listen to it "
                    "as it sorts", ' ', "0.1");
        std::vector<std::string> inputs;
        inputs.push_back("random");
        inputs.push_back("sine");
        inputs.push_back("falling-edge");
        ValuesConstraint<std::string> allowed_inputs(inputs);
        ValueArg<std::string> input_arg("i", "input",
                                        "Input data to be sorted.", false,
                                        "random", &allowed_inputs);
        cmd.add(input_arg);

        std::vector<std::string> modes;
        modes.push_back("swap-pitch");
        modes.push_back("waveform");
        ValuesConstraint<std::string> allowed_modes(modes);
        ValueArg<std::string> mode_arg("m", "mode",
                                       "Choose audibilization mode.", false,
                                       "swap-pitch", &allowed_modes);
        cmd.add(mode_arg);

        std::vector<std::string> sort_names;
        for (auto& it : algorithm_map) {
            sort_names.push_back(it.first);
        }
        ValuesConstraint<std::string> allowed_sorts(sort_names);
        ValueArg<std::string> sort_arg("s", "sort",
                                       "Choose sorting algorithm.", true,
                                       "", &allowed_sorts);
        cmd.add(sort_arg);

        ValueArg<size_t> N_arg("N", "number",
                               "Number of samples to sort (defaults to 50).",
                               false, 50, "#samples");
        cmd.add(N_arg);

        cmd.parse(argc, argv);
        input = input_arg.getValue();
        waveform_mode = (mode_arg.getValue() == "waveform");
        sort = algorithm_map[sort_arg.getValue()];
        N = N_arg.getValue();
    } catch (ArgException& e) {
        std::cerr << e.what() << std::endl;
    }

    // generate input data
    v.resize(N);
    const sample_t max_val = (waveform_mode
                              ? amplitude /* max. amplitude */
                              : 4000 /* max. frequency in Hz */);
    const sample_t min_val = (waveform_mode
                              ? -amplitude
                              : 50 /* min. frequency in Hz */);
    if (input == "random") {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<sample_t> uniform(min_val, max_val);
        for (sample_t& x : v) {
            x = uniform(rng);
        }
    } else if (input == "sine") {
        for (size_t i = 0; i < N; ++i) {
            v[i] = sample_t(max_val * sin(2 * M_PI * i / N));
        }
    } else if (input == "falling-edge") {
        auto slope = 1. * (min_val - max_val) / (N - 1);
        for (size_t i = 0; i < N; ++i) {
            v[i] = sample_t(max_val + slope * i);
        }
    }

    // swap handler
    callback_type<decltype(wav)> swap_handler;
    double phi_1 = 0, phi_2 = 0;
    if (waveform_mode) {
        swap_handler = [&wav,&v](sample_t a, sample_t b) {
            wav.insert(wav.end(), v.begin(), v.end());
        };
    } else {
        swap_handler = [&wav,&phi_1,&phi_2](sample_t a, sample_t b) {
            const size_t l = 1000;
            for (size_t i = 0; i < l; ++i) {
                double p1 = phi_1 + 2 * M_PI * a * i / sample_rate;
                double p2 = phi_2 + 2 * M_PI * b * i / sample_rate;
                double val = sin(p1) + sin(p2);
                val *= .5 * amplitude;
                wav.push_back(sample_t(val));
            }
            phi_1 += 2 * M_PI * a * l / sample_rate;
            phi_2 += 2 * M_PI * b * l / sample_rate;
        };
    }

    // sort the array
    sort(v.begin(), v.end(), swap_handler);

    // play the sound
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return 1;

    static SDL_AudioSpec wav_spec;
    wav_spec.freq = sample_rate;
    wav_spec.format = AUDIO_S16;
    wav_spec.channels = 1;
    wav_spec.samples = 4096;
    wav_spec.silence = 0;
    wav_spec.padding = 0;
    wav_spec.size = 0;
    wav_spec.callback = audio_callback;
    wav_spec.userdata = NULL;

    audio_pos = (uint8_t *)wav.data();
    audio_len = wav.size() * sizeof(sample_t);

    if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
        std::cerr << "Couldn't open audio: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_PauseAudio(0);

    // wait until we're done playing
    while (audio_len > 0) {
        SDL_Delay(100);
    }

    SDL_CloseAudio();

    return 0;
}

void audio_callback(void *userdata, uint8_t *stream, int len) {
    if (audio_len ==0)
        return;

    len = (len > audio_len ? audio_len : len);
    SDL_memset(stream, 0, len);
    SDL_MixAudioFormat(stream, audio_pos, AUDIO_S16, len, SDL_MIX_MAXVOLUME);

    audio_pos += len;
    audio_len -= len;
}
