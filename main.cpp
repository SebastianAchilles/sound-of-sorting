#include <iostream>
#include <functional>
#include <string>
#include <random>
#include <cmath>
#include <cstdint>
#include <limits>

#include <tclap/CmdLine.h>
#include <SDL2/SDL.h>

#include "sorts.hpp"

using namespace TCLAP;

typedef int16_t sample_t;

static uint8_t *audio_pos;
static size_t audio_len;

void audio_callback(void *, uint8_t *, int);

int main(int argc, const char *argv[]) {
    // fixed parameters
    const size_t N = 50;
    const uint32_t sample_rate = 44100;
    const sample_t amplitude = std::numeric_limits<sample_t>::max();
    std::vector<sample_t> v(N);

    // parse command line arguments
    std::string input;
    bool waveform_mode;
    try {
        CmdLine cmd("Apply sorting algorithms to a waveform and listen to it "
                    "as it sorts", ' ', "0.1");
        std::vector<std::string> inputs;
        inputs.push_back("random");
        inputs.push_back("sine");
        inputs.push_back("falling-edge");
        ValuesConstraint<std::string> allowed_inputs(inputs);
        ValueArg<std::string> input_arg("i", "input",
                                        "Input data to be sorted", false,
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

        cmd.parse(argc, argv);
        input = input_arg.getValue();
        waveform_mode = (mode_arg.getValue() == "waveform");
    } catch (ArgException& e) {
        std::cerr << e.what() << std::endl;
    }

    // generate input data
    const sample_t max_val = (waveform_mode
                              ? amplitude /* max. amplitude */
                              : 10000 /* max. frequency in Hz */);
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
    std::vector<sample_t> wav;
    typedef std::vector<sample_t>::const_iterator it_t;
    std::function<void(it_t, it_t)> swap_handler;
    if (waveform_mode) {
        swap_handler = [&wav,&v](it_t it1, it_t it2) {
            wav.insert(wav.end(), v.begin(), v.end());
        };
    } else {
        swap_handler = [&wav](it_t it1, it_t it2) {
            const size_t l = 50000 / N;
            for (size_t i = 0; i < l; ++i) {
                double val = (sin(2 * M_PI * (*it1) * i / sample_rate)
                              + sin(2 * M_PI * (*it2) * i / sample_rate));
                val *= .5 * amplitude;
                wav.push_back(sample_t(val));
            }
        };
    }

    // sort the array
    bubble_sort(v, swap_handler);

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
