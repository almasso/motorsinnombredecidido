#include "AudioMixer.h"
#include "AudioClip.h"

AudioMixer::AudioMixer() :
    _audioDevice(0),
    _output(nullptr),
    _mixers(),
    _clips(),
    _localVolume(1.0f),
    _volume(1.0f) {
}

void AudioMixer::connect(AudioClip* clip) {
    _clips.insert(clip);
    clip->assignMixer(this);
}

void AudioMixer::disconnect(AudioClip* clip) {
    if (_clips.erase(clip) == 0)
        return;
    clip->assignMixer(nullptr);
}

void AudioMixer::connect(AudioMixer* mixer) {
    _mixers.insert(mixer);
    mixer->_output = this;
    mixer->updateVolume();
    assignDevice(_audioDevice);
}

void AudioMixer::disconnect(AudioMixer* mixer) {
    if (_mixers.erase(mixer) == 0)
        return;
    mixer->_output = nullptr;
    mixer->updateVolume();
}

void AudioMixer::setVolume(float volume) {
    _localVolume = volume;
    updateVolume();
}

float AudioMixer::getVolume() const {
    return _localVolume;
}

float AudioMixer::getGlobalVolume() const {
    return _volume;
}

void AudioMixer::updateVolume() {
    _volume = _localVolume * (_output ? _output->_volume : 1.0f);
    for (auto& mixer : _mixers) {
        mixer->updateVolume();
    }
    for (auto& clip : _clips) {
        clip->updateVolume();
    }
}

void AudioMixer::assignDevice(AudioDevice device) {
    if (_output && device != _output->getDevice())
        return;
    _audioDevice = device;
    for (auto& mixer : _mixers) {
        mixer->assignDevice(device);
    }
}

AudioDevice AudioMixer::getDevice() const {
    return _audioDevice;
}
