#include "AudioSource.h"

#include <Core/ComponentData.h>

#include "AudioClip.h"
#include "AudioManager.h"

AudioSource::AudioSource(ComponentData *data) :
    ComponentTemplate(data),
    _clip(nullptr),
    _mixer(nullptr) {
}

AudioSource::~AudioSource() {
    delete _clip;
}

bool AudioSource::init() {
    auto clipKey = _data->getData<std::string>("clip", "");
    if (clipKey.empty())
        return false;
    _clip = new AudioClip(clipKey);
    auto mixerKey = _data->getData<std::string>("mixer", "");
    if (mixerKey.empty())
        return false;
    _mixer = AudioManager::Instance()->getMixer(mixerKey);
    _clip->assignMixer(_mixer);
    _clip->setVolume(_data->getData("volume", 1.0f));
    _clip->setLoop(_data->getData("loop", false));
    return true;
}

void AudioSource::onEnable() {
    pause();
}

void AudioSource::onDisable() {
    resume();
}

void AudioSource::play() {
    _clip->play();
}

void AudioSource::stop() {
    _clip->stop();
}

void AudioSource::pause() {
    _clip->pause();
}

void AudioSource::resume() {
    _clip->resume();
}

bool AudioSource::isPlaying() const {
    return _clip->isPlaying();
}

bool AudioSource::isPaused() const {
    return _clip->isPaused();
}

float AudioSource::getVolume() const {
    return _clip->getVolume();
}

void AudioSource::setVolume(float volume) {
    _clip->setVolume(volume);
}

bool AudioSource::isLooped() const {
    return _clip->isLooped();
}

void AudioSource::setLoop(bool loop) {
    _clip->setLoop(loop);
}
