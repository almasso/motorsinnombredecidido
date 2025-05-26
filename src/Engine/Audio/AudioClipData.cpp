#include "AudioClipData.h"

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>
#include <Utils/Error.h>
#include <SDL3/SDL.h>
#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#else
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif

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
#ifdef __APPLE__
    auto currDir = GetCurrentDir;
    std::string clippath = currDir + _path;
    SDL_free(currDir);
#else
    std::string clippath = _path;
#endif

    if (!SDL_LoadWAV(clippath.c_str(), specifier, &buffer, &bufferLen)) {
        Error::ShowError(std::string("Failed to load") + _path, SDL_GetError());
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
