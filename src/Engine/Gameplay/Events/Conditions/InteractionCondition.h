#ifndef INTERACTIONCONDITION_H
#define INTERACTIONCONDITION_H

#include "../EventCondition.h"

class Collider;

class InteractionCondition : public EventConditionTemplate<"Interaction"> {
public:
    Collider* _interactionArea;
    Collider* _player;
public:
    InteractionCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~InteractionCondition() override;
};



#endif //INTERACTIONCONDITION_H
