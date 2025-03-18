#include "AudioClip.h"
#include <SDL3/SDL_audio.h>

#include "AudioManager.h"
#include "AudioMixer.h"
#include "AudioClipData.h"

#include <iostream>

void AudioClip::Update(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount) {
    auto* instance = static_cast<AudioClip*>(userdata);
    if (additional_amount > 0) {
        if (AudioClipData const* audioData = AudioManager::Instance()->getAudioClipData(instance->key_);
            instance->loop_ && audioData != nullptr) {
            SDL_PutAudioStreamData(stream, audioData->buffer, audioData->bufferLen);
        }
        else if (additional_amount == total_amount)
            instance->stop();
    }
}

void AudioClip::reset() {
    stop();
    SDL_DestroyAudioStream(stream_);
    stream_ = nullptr;
    if (mixer_)
        mixer_->disconnect(this);
    mixer_ = nullptr;
    device_ = 0;
    state_ = STOPPED;
}

AudioClip::AudioClip(AudioClipKey const& key) :
    state_(STOPPED),
    key_(key),
    stream_(nullptr),
    mixer_(nullptr),
    device_(0),
    localVolume_(1.0f),
    volume_(1.0f),
    loop_(false) {
}

AudioClip::~AudioClip() {
    reset();
}

bool AudioClip::play() {
    if (!stop())
        return false;
    if (!stream_)
        return false;
    if (auto data = AudioManager::Instance()->getAudioClipData(key_);
        data == nullptr || !SDL_PutAudioStreamData(stream_, data->buffer, data->bufferLen))
        return false;
    if (!resume())
        return false;
    return true;
}

bool AudioClip::stop() {
    pause();
    if (state_ == STOPPED)
        return true;
    if (!stream_)
        return false;
    if (!SDL_ClearAudioStream(stream_))
        return false;
    state_ = STOPPED;
    return true;
}

void AudioClip::pause() {
    if (state_ != PLAYING)
        return;
    SDL_UnbindAudioStream(stream_);
    state_ = PAUSED;
}

bool AudioClip::resume() {
    if (state_ == PLAYING)
        return true;
    if (!stream_)
        return false;
    if (!SDL_BindAudioStream(device_, stream_))
        return false;
    state_ = PLAYING;
    return true;
}

bool AudioClip::isPlaying() const {
    return state_ == PLAYING;
}

bool AudioClip::isPaused() const {
    return state_ == PAUSED;
}

float AudioClip::getVolume() const {
    return localVolume_;
}

void AudioClip::setVolume(float volume) {
    localVolume_ = volume;
    updateVolume();
}

bool AudioClip::isLooped() const {
    return loop_;
}

void AudioClip::setLoop(bool loop) {
    loop_ = loop;
}

void AudioClip::updateVolume() {
    if (!stream_)
        return;
    volume_ = localVolume_ * mixer_->getGlobalVolume();
    SDL_SetAudioStreamGain(stream_, volume_);
}

void AudioClip::assignMixer(AudioMixer* mixer) {
    if (mixer_)
        mixer_->disconnect(this);
    mixer_ = mixer;
    updateVolume();
    if (!mixer)
        return reset();
    assignDevice(mixer_->getDevice());
}

bool AudioClip::assignDevice(AudioDevice device) {
    if (mixer_ && device != mixer_->getDevice())
        return false;

    auto data = AudioManager::Instance()->getAudioClipData(key_);
    if (data == nullptr)
        return false;

    pause();
    device_ = device;
    SDL_AudioSpec dstSpec;
    SDL_GetAudioDeviceFormat(device_, &dstSpec, NULL);
    if (!stream_) stream_ = SDL_CreateAudioStream(data->specifier, &dstSpec);
    else SDL_SetAudioStreamFormat(stream_, data->specifier, &dstSpec);
    SDL_SetAudioStreamGetCallback(stream_, AudioClip::Update, this);
    resume();
    return true;
}