#include "AudioClip.h"
#include <SDL3/SDL.h>

#include "AudioMixer.h"

void AudioClip::reset() {
    stop();
    SDL_DestroyAudioStream(stream_);
    stream_ = nullptr;
    mixer_ = nullptr;
    device_ = 0;
    state_ = STOPPED;
}

AudioClip::AudioClip(AudioData* buffer, uint32_t bufferLen, AudioSpec* specifier) :
    state_(STOPPED),
    buffer_(buffer),
    bufferLen_(bufferLen),
    specifier_(specifier),
    stream_(nullptr),
    mixer_(nullptr),
    device_(0),
    localVolume_(1.0f),
    volume_(1.0f) {
}

AudioClip::~AudioClip() {
    reset();
    SDL_free(buffer_);
    delete specifier_;
}

bool AudioClip::play() {
    if (!stop())
        return false;
    if (!stream_)
        return false;
    if (!SDL_PutAudioStreamData(stream_, buffer_, bufferLen_))
        return false;
    if (!resume())
        return false;
    return true;
}

bool AudioClip::stop() {
    pause();
    if (state_ == STOPPED)
        return true;
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
    if (!SDL_BindAudioStream(device_, stream_))
        return false;
    state_ = PLAYING;
    return true;
}

bool AudioClip::isPlaying() const {
    return state_ == PLAYING;
}

float AudioClip::getVolume() const {
    return localVolume_;
}

void AudioClip::setVolume(float volume) {
    localVolume_ = volume;
    updateVolume();
}

void AudioClip::updateVolume() {
    if (!stream_)
        return;
    volume_ = localVolume_ * mixer_->getGlobalVolume();
    SDL_SetAudioStreamGain(stream_, volume_);
}

void AudioClip::assignMixer(AudioMixer* mixer) {
    if (!mixer)
        return reset();

    mixer_ = mixer;
    assignDevice(mixer_->getDevice());
}

void AudioClip::assignDevice(AudioDevice device) {
    pause();
    device_ = device;
    SDL_AudioSpec dstSpec;
    SDL_GetAudioDeviceFormat(device_, &dstSpec, NULL);
    if (!stream_) stream_ = SDL_CreateAudioStream(specifier_, &dstSpec);
    else SDL_SetAudioStreamFormat(stream_, specifier_, &dstSpec);
    resume();
}