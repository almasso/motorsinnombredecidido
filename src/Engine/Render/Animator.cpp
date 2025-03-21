#include "Animator.h"
#include "Animation.h"
#include <Utils/Time.h>

bool Animator::update() {
  if(!_animationEnded) {
    _frameTimer += Time::deltaTime;
    Animation* anim = new Animation(_animation);
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
