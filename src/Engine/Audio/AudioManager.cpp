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

AudioManager* AudioManager::instance = nullptr;

AudioManager::AudioManager(ResourceMemoryManager* resourceMemoryManager) :
    audioDeviceId_(0),
    clipDataHandler_(resourceMemoryManager){
}

AudioManager::~AudioManager() = default;

bool AudioManager::initTest() {
    AudioMixerData mixerData;
    mixerData.name = "Master";
    registerAudioMixer(mixerData);

    AudioClipData* data = new AudioClipData("assets/SodaLoop.wav");
    if (!data->load())
        return false;

    clipDataHandler_.add("sonido", "assets/SodaLoop.wav");
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

    audioDeviceId_ = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audioDeviceId_ == 0) {
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
    SDL_CloseAudioDevice(audioDeviceId_);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioManager::Init(ResourceMemoryManager* resourceMemoryManager) {
    assert(instance == nullptr);
    instance = new AudioManager(resourceMemoryManager);
    if (instance->init())
        return true;
    Shutdown();
    return false;
}

AudioManager* AudioManager::Instance() {
    return instance;
}

void AudioManager::Shutdown() {
    instance->shutdown();
    delete instance;
}

bool AudioManager::registerAudioMixer(AudioMixerData const& data) {
    auto [it, inserted] = mixers_.insert({data.name, nullptr});
    if (!inserted)
        return false;
    it->second = new AudioMixer();
    it->second->assignDevice(audioDeviceId_);
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

AudioClipData const* AudioManager::getAudioClipData(AudioClipKey const& key) {
    return clipDataHandler_.get(key);
}

AudioClip* AudioManager::createAudioClip(AudioClipKey const& key) {
    auto* clip = new AudioClip(key);
    clipNames_.insert({clip, key});
    return clip;
}

void AudioManager::releaseAudioClip(AudioClip* clip) {
    if (!clip)
        return;
    auto it = clipNames_.find(clip);
    if (it == clipNames_.end())
        return;
    clipNames_.erase(it);
    delete clip;
}
