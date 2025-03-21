#ifndef ANIMATION_H
#define ANIMATION_H
#include "Sprite.h"
#include <vector>

struct Animation {
  std::vector<const Sprite*> frames;
  float frameTime;
  bool loop;
  int numFrames;
  Animation(std::vector<const Sprite*> frms = {}, float frmTime = 0.0f, bool lp = false)
      : frames(std::move(frms)), frameTime(frmTime), loop(lp) {
    numFrames = static_cast<int>(frames.size());
  }
};

#endif //ANIMATION_H
