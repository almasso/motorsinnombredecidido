#ifndef MOMEMENTCOMPONENT_H
#define MOMEMENTCOMPONENT_H
#include "MovementObstacle.h"

class MovementComponent : public ComponentTemplate<"MovementComponent",MovementObstacle> {
  private:
  std::vector<Vector2> _path;
  float _speed;
  int _pathIndex;
  public:
  MovementComponent(ComponentData const* data);
  bool init() override;
  bool update() override;
  void setTarget(const Vector2& target);

};

#endif //MOMEMENTCOMPONENT_H
