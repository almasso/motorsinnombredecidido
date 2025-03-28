#include "AudioClip.h"
#include <SDL3/SDL_audio.h>

#include "AudioMixer.h"
#include "AudioClipData.h"

#include <iostream>
#include <Load/ResourceHandler.h>

void AudioClip::Update(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount) {
    auto* instance = static_cast<AudioClip*>(userdata);
    if (additional_amount > 0) {
        if (AudioClipData const* audioData = ResourceHandler<AudioClipData>::Instance()->get(instance->_key);
            instance->_loop && audioData != nullptr) {
            SDL_PutAudioStreamData(stream, audioData->buffer, audioData->bufferLen);
        }
        else if (additional_amount == total_amount)
            instance->stop();
    }
}

void AudioClip::reset() {
    stop();
    SDL_DestroyAudioStream(_stream);
    _stream = nullptr;
    if (_mixer)
        _mixer->disconnect(this);
    _mixer = nullptr;
    _device = 0;
    _state = STOPPED;
}

AudioClip::AudioClip(std::string const& key) :
    _state(STOPPED),
    _key(key),
    _stream(nullptr),
    _mixer(nullptr),
    _device(0),
    _localVolume(1.0f),
    _volume(1.0f),
    _loop(false) {
}

AudioClip::~AudioClip() {
    reset();
}

bool AudioClip::play() {
    if (!stop())
        return false;
    if (!_stream)
        return false;
    if (auto data = ResourceHandler<AudioClipData>::Instance()->get(_key);
        data == nullptr || !SDL_PutAudioStreamData(_stream, data->buffer, data->bufferLen))
        return false;
    if (!resume())
        return false;
    return true;
}

bool AudioClip::stop() {
    pause();
    if (_state == STOPPED)
        return true;
    if (!_stream)
        return false;
    if (!SDL_ClearAudioStream(_stream))
        return false;
    _state = STOPPED;
    return true;
}

void AudioClip::pause() {
    if (_state != PLAYING)
        return;
    SDL_UnbindAudioStream(_stream);
    _state = PAUSED;
}

bool AudioClip::resume() {
    if (_state == PLAYING)
        return true;
    if (!_stream)
        return false;
    if (!SDL_BindAudioStream(_device, _stream))
        return false;
    _state = PLAYING;
    return true;
}

bool AudioClip::isPlaying() const {
    return _state == PLAYING;
}

bool AudioClip::isPaused() const {
    return _state == PAUSED;
}

float AudioClip::getVolume() const {
    return _localVolume;
}

void AudioClip::setVolume(float volume) {
    _localVolume = volume;
    updateVolume();
}

bool AudioClip::isLooped() const {
    return _loop;
}

void AudioClip::setLoop(bool loop) {
    _loop = loop;
}

void AudioClip::updateVolume() {
    if (!_stream)
        return;
    _volume = _localVolume * _mixer->getGlobalVolume();
    SDL_SetAudioStreamGain(_stream, _volume);
}

void AudioClip::assignMixer(AudioMixer* mixer) {
    if (_mixer)
        _mixer->disconnect(this);
    _mixer = mixer;
    updateVolume();
    if (!mixer)
        return reset();
    assignDevice(_mixer->getDevice());
}

bool AudioClip::assignDevice(AudioDevice device) {
    if (_mixer && device != _mixer->getDevice())
        return false;

    auto data = ResourceHandler<AudioClipData>::Instance()->get(_key);
    if (data == nullptr)
        return false;

    pause();
    _device = device;
    SDL_AudioSpec dstSpec;
    SDL_GetAudioDeviceFormat(_device, &dstSpec, NULL);
    if (!_stream) _stream = SDL_CreateAudioStream(data->specifier, &dstSpec);
    else SDL_SetAudioStreamFormat(_stream, data->specifier, &dstSpec);
    SDL_SetAudioStreamGetCallback(_stream, AudioClip::Update, this);
    resume();
    return true;
}