#include "AudioClipData.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include <Utils/RPGError.h>

AudioClipData::AudioClipData(std::string const& path) :
    Resource(path),
    buffer(nullptr),
    bufferLen(0),
    specifier(nullptr) {
}

AudioClipData::~AudioClipData() {
    unload();
}

bool AudioClipData::load() {
    specifier = new SDL_AudioSpec();
    if (!SDL_LoadWAV(_path.c_str(), specifier, &buffer, &bufferLen)) {
        RPGError::ShowError(std::string("Failed to load") + _path, SDL_GetError());
        return false;
    }
    _size = bufferLen;
    return true;
}

void AudioClipData::unload() {
    delete specifier;
    SDL_free(buffer);
    specifier = nullptr;
    buffer = nullptr;
    bufferLen = -1;
    _size = -1;
}
