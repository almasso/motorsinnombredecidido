#include "AudioManager.h"

#include <assert.h>
#include <cstdio>
#include <Load/ResourceHandler.h>
#include <SDL3/SDL.h>

#include <Utils/RPGError.h>
#include <Utils/Time.h>
#include "AudioClip.h"
#include "AudioMixer.h"
#include "AudioMixerData.h"

AudioManager* AudioManager::_instance = nullptr;

AudioManager::AudioManager() :
    _audioDeviceId(0) {
}

AudioManager::~AudioManager() {
    for (auto& mixer : _mixers) {
        delete mixer.second;
    }
    SDL_CloseAudioDevice(_audioDeviceId);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioManager::init() {
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        RPGError::ShowError("Audio Subsystem initialization failed", SDL_GetError());
        return false;
    }

    _audioDeviceId = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (_audioDeviceId == 0) {
        RPGError::ShowError("Audio Device initialization failed", SDL_GetError());
        return false;
    }

    return true;
}

bool AudioManager::Init() {
    assert(_instance == nullptr);
    _instance = new AudioManager();
    if (_instance->init())
        return true;
    Shutdown();
    return false;
}

AudioManager* AudioManager::Instance() {
    return _instance;
}

void AudioManager::Shutdown() {
    delete _instance;
}

AudioMixer* AudioManager::registerAudioMixer(AudioMixerData const* data) {
    auto [it, inserted] = _mixers.insert({data->name, nullptr});
    if (!inserted)
        return it->second;
    it->second = new AudioMixer();
    it->second->setVolume(data->volume);
    it->second->assignDevice(_audioDeviceId);
    if (AudioMixer* output = getMixer(data->output))
        output->connect(it->second);
    for (auto& in : data->inputs) {
        if (AudioMixer* input = getMixer(in))
            it->second->connect(input);
    }
    return it->second;
}

AudioMixer* AudioManager::getMixer(std::string const& mixer) {
    if (mixer == "")
        return nullptr;
    return registerAudioMixer(ResourceHandler<AudioMixerData>::Instance()->get(mixer));
}