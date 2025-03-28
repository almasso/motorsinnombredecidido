#include "Animator.h"
#include "Animation.h"
#include <Core/ComponentData.h>
#include <Load/ResourceHandler.h>
#include <Utils/Time.h>

Animator::Animator(ComponentData const*data) : ComponentTemplate(data) {
}

bool Animator::init() {
  _animation = _data->getData<std::string>("animation");
  const Animation* anim = ResourceHandler<Animation>::Instance()->get(_animation);
  if (!anim)
    return false;
  if (!setSprite(anim->frames[_currentFrame]))
    return false;
  _playing = _data->getData<bool>("playing", false);

  return RenderComponent::init();
}

bool Animator::update() {
  if(_playing && !_animationEnded) {
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
}

void Animator::changeAnimation(const std::string& animation) {
  _animation = animation;
  reset();
}

void Animator::setPlaying(bool playing) {
  _playing = playing;
}
