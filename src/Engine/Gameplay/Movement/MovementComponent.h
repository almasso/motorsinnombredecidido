#ifndef MOMEMENTCOMPONENT_H
#define MOMEMENTCOMPONENT_H

#include "MovementObstacle.h"
#include <string>
#include <array>

class Animator;

class ComponentDerived(MovementComponent, MovementObstacle) {
  private:
  std::vector<Vector2> _path;
  std::array<std::string, 4> _animations;
  float _speed;
  int _pathIndex;
  Animator* _animator;
  public:
  MovementComponent(ComponentData const* data);
  bool init() override;
  bool update() override;
  void setTarget(const Vector2& target);

  static void RegisterToLua(sol::state& lua);
};

#endif //MOMEMENTCOMPONENT_H
