#ifndef MOMEMENTCOMPONENT_H
#define MOMEMENTCOMPONENT_H
#include "MovementObstacle.h"

class ComponentDerived(MovementComponent, MovementObstacle) {
  private:
  std::vector<Vector2> _path;
  float _speed;
  int _pathIndex;
  public:
  MovementComponent(ComponentData const* data);
  bool init() override;
  bool update() override;
  void setTarget(const Vector2& target);

  static void RegisterToLua(sol::state& lua);
};

#endif //MOMEMENTCOMPONENT_H
