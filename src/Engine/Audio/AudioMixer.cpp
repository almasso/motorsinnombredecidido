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
    clips_.insert(clip);
    clip->assignMixer(this);
}

void AudioMixer::disconnect(AudioClip* clip) {
    if (clips_.erase(clip) == 0)
        return;
    clip->assignMixer(nullptr);
}

void AudioMixer::connect(AudioMixer* mixer) {
    mixers_.insert(mixer);
    mixer->output_ = this;
    mixer->updateVolume();
    assignDevice(audioDevice_);
}

void AudioMixer::disconnect(AudioMixer* mixer) {
    if (mixers_.erase(mixer) == 0)
        return;
    mixer->output_ = nullptr;
    mixer->updateVolume();
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
    if (output_ && device != output_->getDevice())
        return;
    audioDevice_ = device;
    for (auto& mixer : mixers_) {
        mixer->assignDevice(device);
    }
}

AudioDevice AudioMixer::getDevice() const {
    return audioDevice_;
}
