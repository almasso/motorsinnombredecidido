#include "AudioManager.h"

#include <assert.h>
#include <cstdio>
#include <SDL3/SDL.h>

#include <Utils/RPGError.h>
#include <Utils/Time.h>
#include "AudioClip.h"
#include "AudioClipData.h"
#include "AudioMixer.h"
#include "AudioMixerData.h"

AudioManager* AudioManager::_instance = nullptr;

AudioManager::AudioManager() :
    _audioDeviceId(0) {
}

AudioManager::~AudioManager() {
    shutdownTest();

    for (auto& mixer : _mixers) {
        delete mixer.second;
    }
    SDL_CloseAudioDevice(_audioDeviceId);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioManager::initTest() {
    AudioMixerData mixerData;
    mixerData.name = "Master";
    registerAudioMixer(mixerData);

    testClip_ = createAudioClip("assets/SodaLoop.wav");
    _mixers["Master"]->connect(testClip_);

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

    return initTest();
}

void AudioManager::update() {
    updateTest();
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

bool AudioManager::registerAudioMixer(AudioMixerData const& data) {
    auto [it, inserted] = _mixers.insert({data.name, nullptr});
    if (!inserted)
        return false;
    it->second = new AudioMixer();
    it->second->assignDevice(_audioDeviceId);
    if (AudioMixer* output = getMixer(data.output))
        output->connect(it->second);
    for (auto& in : data.inputs) {
        if (AudioMixer* input = getMixer(in))
            it->second->connect(input);
    }
    return true;
}

AudioMixer* AudioManager::getMixer(std::string const& mixer) {
    auto mix = _mixers.find(mixer);
    if (mix == _mixers.end()) return nullptr;
    return mix->second;
}

AudioClip* AudioManager::createAudioClip(AudioClipKey const& key) {
    auto* clip = new AudioClip(key);
    _clipNames.insert({clip, key});
    return clip;
}

void AudioManager::releaseAudioClip(AudioClip* clip) {
    if (!clip)
        return;
    auto it = _clipNames.find(clip);
    if (it == _clipNames.end())
        return;
    _clipNames.erase(it);
    delete clip;
}
