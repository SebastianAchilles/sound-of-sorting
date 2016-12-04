#include <iostream>
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
    const size_t N = 100;
    const uint32_t sample_rate = 44100;
    std::vector<sample_t> v(N);

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

    // generate input data
    const sample_t max_val = std::numeric_limits<sample_t>::max();
    if (waveform == "white-noise") {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<sample_t> uniform(-max_val, max_val);
        for (sample_t& x : v) {
            x = uniform(rng);
        }
    } else if (waveform == "sine") {
        for (size_t i = 0; i < N; ++i) {
            v[i] = sample_t(max_val * sin(2 * M_PI * i / N));
        }
    }

    // sort the array; keep snapshots
    std::vector<sample_t> acc;
    bubble_sort(v, [&acc,&v]() {
            acc.insert(acc.end(), v.begin(), v.end());
        });

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

    audio_pos = (uint8_t *)acc.data();
    audio_len = acc.size() * sizeof(sample_t);

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
