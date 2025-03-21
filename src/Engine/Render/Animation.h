#ifndef ANIMATION_H
#define ANIMATION_H
#include "Sprite.h"
#include <vector>

class Animation {
  private:
  std::vector<Sprite*> _frames;
  float _frameTime;
  bool _loop;
  int _numFrames;
  float _frameTimer;
  int _currentFrame;
  public:
  Animation(const std::vector<Sprite*>& frames, float frameTime, bool loop);
  void update(float deltaTime);
  void reset();
  Sprite* getCurrentFrame() const;
};

#endif //ANIMATION_H
