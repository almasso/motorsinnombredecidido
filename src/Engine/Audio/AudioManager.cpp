#include "AudioManager.h"

#include <cstdio>
#include <SDL3/SDL.h>

#include <Utils/RPGError.h>
#include <Utils/Time.h>
#include "AudioClip.h"
#include "AudioClipData.h"
#include "AudioMixer.h"
#include "AudioMixerData.h"

AudioManager::AudioManager() :
    audioDeviceId(0) {
}

AudioManager::~AudioManager() = default;

bool AudioManager::initTest() {
    AudioMixerData mixerData;
    mixerData.name = "Master";
    registerAudioMixer(mixerData);

    AudioClipData* data = new AudioClipData();
    data->specifier = new SDL_AudioSpec();
    if (!SDL_LoadWAV("assets/SodaLoop.wav", data->specifier, &data->buffer, &data->bufferLen)) {
        RPGError::ShowError(std::string("Failed to load") + "assets/audio.wav", SDL_GetError());
        return false;
    }

    clipDataHandler_.add("sonido", data->buffer, data->bufferLen, data->specifier);
    clipDataHandler_.addReady("sonido", data);
    testClip_ = createAudioClip("sonido");
    mixers_["Master"]->connect(testClip_);

    testClip_->play();
    return true;
}

void AudioManager::updateTest() {
    if (Time::time > 30)
        testClip_->resume();
    else if (Time::time > 20)
        testClip_->pause();
}

void AudioManager::shutdownTest() {
    releaseAudioClip(testClip_);
    clipDataHandler_.flush();
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

    return initTest();
}

void AudioManager::update() {
    updateTest();
}

void AudioManager::shutdown() {
    shutdownTest();

    for (auto& mixer : mixers_) {
        delete mixer.second;
    }
    SDL_CloseAudioDevice(audioDeviceId);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioManager::registerAudioMixer(AudioMixerData const& data) {
    auto [it, inserted] = mixers_.insert({data.name, nullptr});
    if (!inserted)
        return false;
    it->second = new AudioMixer();
    it->second->assignDevice(audioDeviceId);
    if (AudioMixer* output = getMixer(data.output))
        output->connect(it->second);
    for (auto& in : data.inputs) {
        if (AudioMixer* input = getMixer(in))
            it->second->connect(input);
    }
    return true;
}

AudioMixer* AudioManager::getMixer(std::string const& mixer) {
    auto mix = mixers_.find(mixer);
    if (mix == mixers_.end()) return nullptr;
    return mix->second;
}

AudioClip* AudioManager::createAudioClip(std::string const& key) {
    auto const* data = clipDataHandler_.get(key);
    if (!data)
        return nullptr;
    auto* clip = new AudioClip(data);
    clipNames_.insert({clip, key});
    return clip;
}

void AudioManager::releaseAudioClip(AudioClip* clip) {
    if (!clip)
        return;
    auto it = clipNames_.find(clip);
    if (it == clipNames_.end())
        return;
    clipDataHandler_.dereference(it->second);
    clipNames_.erase(it);
    delete clip;
}
