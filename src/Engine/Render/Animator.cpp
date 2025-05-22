#include "Animator.h"
#include "Animation.h"
#include <Core/ComponentData.h>
#include <Load/ResourceHandler.h>
#include <Utils/Time.h>
#include <sol/state.hpp>

Animator::Animator(ComponentData const*data) : ComponentTemplate(data) {
}

bool Animator::init() {
  _animation = _data->getData<std::string>("animation", "");
  _defaultSprite = _data->getData<std::string>("sprite", "");
  if (!_defaultSprite.empty()) {
    if (!setSprite(_defaultSprite)) {
      return false;
    }
  }
  if (!_animation.empty()) {
    const Animation* anim = ResourceHandler<Animation>::Instance()->get(_animation);
    if (!anim)
      return false;
    if (!setSprite(anim->frames[_currentFrame]))
      return false;
  }
  _playing = _data->getData<bool>("playing", false);

  return RenderComponent::init();
}

bool Animator::update() {
  if(_playing && !_animationEnded) {
    if (_animation.empty()) {
      _animationEnded = true;
      if (!_defaultSprite.empty()) {
        return setSprite(_defaultSprite);
      }
      return true;
    }
    _frameTimer += Time::deltaTime;
    const Animation* anim = ResourceHandler<Animation>::Instance()->get(_animation);
    if(!anim) return false;
    while (_frameTimer >= anim->frameTime) {
        _frameTimer -= anim->frameTime;
        _currentFrame++;
        if (_currentFrame >= anim->numFrames) {
          if(anim->loop) {
            _currentFrame %= anim->numFrames;
          }
          else {
            _currentFrame = anim->numFrames - 1;
            _animationEnded = true;
            if (!_defaultSprite.empty()) {
              return setSprite(_defaultSprite);
            }
          }
        }
        return setSprite(anim->frames[_currentFrame]);
    }
  }
  return true;
}

void Animator::reset() {
  _animationEnded = false;
  _currentFrame = 0;
  _frameTimer = 0;
  _playing = true;
}

void Animator::changeAnimation(const std::string& animation) {
  if (_animation != animation) {
    _animation = animation;
    reset();
  }
}

void Animator::setPlaying(bool playing) {
  _playing = playing;
}

bool Animator::animationEnded() const {
    return _animationEnded;
}

void Animator::RegisterToLua(sol::state& lua) {
    sol::usertype<Animator> type = lua.new_usertype<Animator>("Animator");
    type["reset"] = &Animator::reset;
    type["changeAnimation"] = &Animator::changeAnimation;
    type["setPlaying"] = &Animator::setPlaying;
    type["animationEnded"] = &Animator::animationEnded;
    type["get"] = Animator::get;
}
