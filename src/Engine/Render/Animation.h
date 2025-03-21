#ifndef ANIMATION_H
#define ANIMATION_H
#include <vector>
#include <Load/Resource.h>

class Animation : public Resource {
public:
  std::vector<std::string> frames;
  float frameTime;
  bool loop;
  int numFrames;
  Animation(std::string const &path);
  bool load() override;
  void unload() override;
};
#endif //ANIMATION_H
