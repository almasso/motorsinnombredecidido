#include "Animator.h"

#include <Load/ResourceHandler.h>

#include "Animation.h"
#include <Utils/Time.h>

Animator::Animator(ComponentData *data) : ComponentTemplate(data) {
}

bool Animator::update() {
  if(!_animationEnded) {
    _frameTimer += Time::deltaTime;
    const Animation* anim = ResourceHandler<Animation>::Instance()->get(_animation);
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
        setSprite(anim->frames[_currentFrame]);
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
