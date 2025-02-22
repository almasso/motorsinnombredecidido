#include "AudioManager.h"

#include <cstdio>
#include <SDL3/SDL.h>

#include <Utils/RPGError.h>
#include <Utils/Time.h>
#include "AudioClip.h"
#include "AudioMixer.h"

AudioManager::AudioManager() :
    audioDeviceId(0) {
}

AudioManager::~AudioManager() {
}

bool AudioManager::init() {
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        RPGError::ShowError("Audio Subsystem initialization failed", SDL_GetError());
        return false;
    }

    audioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audioDeviceId == 0) {
        RPGError::ShowError("Audio Device initialization failed", SDL_GetError());
        return false;
    }

    mixers_["Master"] = new AudioMixer();
    mixers_["Master"]->assignDevice(audioDeviceId);

    SDL_AudioSpec* srcSpec = new SDL_AudioSpec();
    Uint8* srcBuffer;
    Uint32 srcLen;
    if (!SDL_LoadWAV("assets/audio.wav", srcSpec, &srcBuffer, &srcLen)) {
        RPGError::ShowError(std::string("Failed to load") + "assets/audio.wav", SDL_GetError());
        return false;
    }

    testClip_ = new AudioClip(srcBuffer, srcLen, srcSpec);
    mixers_["Master"]->connect(testClip_);

    testClip_->play();
    return true;
}

void AudioManager::update() {
    if (Time::time > 20)
        testClip_->resume();
    else if (Time::time > 15)
        testClip_->pause();
    else if (Time::time > 5 && testFlag_) {
        testFlag_ = false;
        testClip_->play();
    }
    else if (Time::time > 3 && testFlag_)
        testClip_->stop();
}

void AudioManager::shutdown() {
    SDL_CloseAudioDevice(audioDeviceId);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
