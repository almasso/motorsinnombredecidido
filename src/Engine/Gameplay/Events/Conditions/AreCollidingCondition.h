#ifndef ARECOLLIDINGCONDITION_H
#define ARECOLLIDINGCONDITION_H

#include "../EventCondition.h"

class Collider;

class AreCollidingCondition : public EventConditionTemplate<"AreColliding"> {
private:
    Collider* _colliderA;
    Collider* _colliderB;

    bool initCollider(Collider*& collider, std::string const& handler);
public:
    AreCollidingCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~AreCollidingCondition() override;
};



#endif //ARECOLLIDINGCONDITION_H
