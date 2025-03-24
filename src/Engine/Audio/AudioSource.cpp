#include "AudioSource.h"

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
    _clip = new AudioClip("");
    _mixer = AudioManager::Instance()->getMixer("Master");
    _clip->assignMixer(_mixer);
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
