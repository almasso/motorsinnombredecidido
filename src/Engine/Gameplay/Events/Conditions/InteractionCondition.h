#ifndef INTERACTIONCONDITION_H
#define INTERACTIONCONDITION_H

#include <Utils/Rect.h>

#include "../EventCondition.h"

class Camera;
class Vector2;
class Collider;

class InteractionCondition : public EventConditionTemplate<"Interaction"> {
private:
    Collider* _interactionArea;
    Collider* _player;
    Camera* _camera;

    static bool clickInside(const Rect& rect, const Vector2& clickPos);
public:
    InteractionCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~InteractionCondition() override;
};



#endif //INTERACTIONCONDITION_H
