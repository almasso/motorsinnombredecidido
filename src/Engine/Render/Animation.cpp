#include "Animation.h"

Animation::Animation(const std::vector<Sprite*>& frames, const float frameTime, const bool loop) :
_frames(frames),
_frameTime(frameTime),
_loop(loop),
_currentFrame(0),
_frameTimer(0),
_numFrames(static_cast<int>(_frames.size())){
}

void Animation::update(const float deltaTime) {
    _frameTimer += deltaTime;
    while (_frameTimer >= _frameTime) {
        _frameTimer -= _frameTime;
        ++_currentFrame %= _numFrames;
    }
}

void Animation::reset() {
    _frameTimer = 0;
    _currentFrame = 0;
}

Sprite * Animation::getCurrentFrame() const {
    return _frames[_currentFrame];
}
