#include "AudioSource.h"

#include <Core/ComponentData.h>

#include "AudioClip.h"
#include "AudioManager.h"

AudioSource::AudioSource(ComponentData const*data) :
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
    if (_data->getData("playOnAwake", false))
        play();
    return true;
}

void AudioSource::onEnable() {
    resume();
}

void AudioSource::onDisable() {
    pause();
}

bool AudioSource::play() {
    return _clip->play();
}

bool AudioSource::stop() {
    return _clip->stop();
}

void AudioSource::pause() {
    _clip->pause();
}

bool AudioSource::resume() {
    return _clip->resume();
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

void AudioSource::changeClip(std::string const& key) {
    _clip->changeKey(key);
}

void AudioSource::RegisterToLua(sol::state& luaState) {
    sol::usertype<AudioSource> type = luaState.new_usertype<AudioSource>("AudioSource");
    type["play"] = &AudioSource::play;
    type["stop"] = &AudioSource::stop;
    type["pause"] = &AudioSource::pause;
    type["resume"] = &AudioSource::resume;
    type["isPlaying"] = &AudioSource::isPlaying;
    type["isPaused"] = &AudioSource::isPaused;
    type["isLooped"] = &AudioSource::isLooped;
    type["getVolume"] = &AudioSource::getVolume;
    type["setVolume"] = &AudioSource::setVolume;
    type["isLooped"] = &AudioSource::isLooped;
    type["setLoop"] = &AudioSource::setLoop;
    type["changeClip"] = &AudioSource::changeClip;
    type["get"] = AudioSource::get;
}
