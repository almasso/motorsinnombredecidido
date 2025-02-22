#include "AudioMixer.h"
#include "AudioClip.h"

AudioMixer::AudioMixer() :
    audioDevice_(0),
    output_(nullptr),
    mixers_(),
    clips_(),
    localVolume_(1.0f),
    volume_(1.0f) {
}

void AudioMixer::connect(AudioClip* clip) {
    clips_.push_back(clip);
    clip->assignMixer(this);
}

void AudioMixer::connect(AudioMixer* mixer) {
    mixers_.push_back(mixer);
    mixer->output_ = this;
    assignDevice(audioDevice_);
}

void AudioMixer::setVolume(float volume) {
    localVolume_ = volume;
    updateVolume();
}

float AudioMixer::getGlobalVolume() const {
    return volume_;
}

void AudioMixer::updateVolume() {
    volume_ = localVolume_ * (output_ ? output_->volume_ : 1.0f);
    for (auto& mixer : mixers_) {
        mixer->updateVolume();
    }
    for (auto& clip : clips_) {
        clip->updateVolume();
    }
}

void AudioMixer::assignDevice(AudioDevice device) {
    audioDevice_ = device;
    for (auto& mixer : mixers_) {
        mixer->assignDevice(device);
    }
}

AudioDevice AudioMixer::getDevice() const {
    return audioDevice_;
}
