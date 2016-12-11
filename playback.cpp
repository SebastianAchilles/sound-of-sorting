#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>

#include <SDL2/SDL.h>

extern std::string song;

static uint8_t *song_pos;
static size_t song_len;

std::vector<std::string> poem = {
    "Hello chaos, my old friend",
    "I've come to get rid of you again",
    "Because a random number engine",
    "Left its seed while init'lizing",
    "And the entropy was planted in Markov chains",
    "Still remains",
    "Within the sound of sorting\n",
    "In Minecraft worlds I walked alone",
    "Narrow streets of cobblestone",
    "'Neath the root of an ordered heap",
    "I swapped its subtrees for quite cheap",
    "When my ears were stabbed by the chirp of white noise",
    "I rejoiced",
    "And produced the sound of sorting\n",
    "And in the memory I saw",
    "Ten thousand samples, maybe more",
    "People codin' without typin'",
    "People debuggin' without breakin'",
    "People writing code that through github they share",
    "And no one dared",
    "Disturb the sound of sorting\n"
};

std::vector<double> timings = {
    03.628,
    07.633,
    11.978,
    17.500,
    22.158,
    29.086,
    32.327,
    38.236,
    43.034,
    48.306,
    52.681,
    57.395,
    64.136,
    67.518,
    74.136,
    77.838,
    83.163,
    87.601,
    93.000,
    99.156,
    103.00
};

size_t current_idx = 0;
int total_len;
uint32_t srate;

void song_callback(void *userdata, uint8_t *stream, int len) {
    if (song_len ==0)
        return;

    len = (len > song_len ? song_len : len);
    SDL_memset(stream, 0, len);
    SDL_MixAudioFormat(stream, song_pos, AUDIO_S16, len, SDL_MIX_MAXVOLUME);

    double t = .25 * (total_len - song_len) / srate;
    while (current_idx < timings.size() && t > timings[current_idx]) {
        std::cout << poem[current_idx] << std::endl;
        ++current_idx;
    }

    song_pos += len;
    song_len -= len;
}

int playback() {
    std::ofstream f("/tmp/silence.txt");
    f << song;
    f.close();

    std::system("base64 -d /tmp/silence.txt > /tmp/silence.wav");

    // play the sound
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return 1;

    static SDL_AudioSpec wav_spec;
    static uint32_t wav_length; // length of our sample
	static uint8_t *wav_buffer; // buffer containing our audio file

    if (SDL_LoadWAV("/tmp/silence.wav", &wav_spec, &wav_buffer, &wav_length) == NULL) {
        return 1;
	}
	wav_spec.callback = song_callback;
	wav_spec.userdata = NULL;

    song_pos = wav_buffer;
    song_len = wav_length;
    total_len = wav_length;
    srate = wav_spec.freq;

    if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
        std::cerr << "Couldn't open audio: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_PauseAudio(0);

    // wait until we're done playing
    while (song_len > 0) {
        SDL_Delay(100);
    }

    SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);

    std::system("rm -f /tmp/silence.txt /tmp/silence.wav");
    return 0;
}
    
