#ifndef ARECOLLIDINGCONDITION_H
#define ARECOLLIDINGCONDITION_H

#include "../EventCondition.h"

class Collider;

class CollidesWithPlayerCondition : public EventConditionTemplate<"CollidesWithPlayer"> {
private:
    Collider* _collider;
    Collider* _playerCollider;

    static bool initCollider(Collider*& collider, Entity* entity);
public:
    CollidesWithPlayerCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~CollidesWithPlayerCondition() override;
};



#endif //ARECOLLIDINGCONDITION_H
