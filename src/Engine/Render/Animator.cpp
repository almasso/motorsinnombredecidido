#include "Animator.h"
#include <Utils/Time.h>

const std::string Animator::id = "Animator";

bool Animator::update() {
  if(!_animationEnded) {
    _frameTimer += Time::deltaTime;
    while (_frameTimer >= _animation->frameTime) {
        _frameTimer -= _animation->frameTime;
        _currentFrame++;
        if (_currentFrame >= _animation->numFrames) {
          if(_animation->loop) {
            _currentFrame %= _animation->numFrames;
          }
          else {
              _currentFrame = _animation->numFrames - 1;
              _animationEnded = true;
          }
        }
        setSprite(_animation->frames[_currentFrame]);
    }
  }
  return true;
}

void Animator::reset() {
  _animationEnded = false;
  _currentFrame = 0;
  _frameTimer = 0;
}

void Animator::changeAnimation(const Animation *animation) {
  _animation = animation;
  reset();
}
