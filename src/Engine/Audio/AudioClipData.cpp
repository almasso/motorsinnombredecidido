#include "AudioClipData.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>

AudioClipData::~AudioClipData() {
    delete specifier;
    SDL_free(buffer);
}
