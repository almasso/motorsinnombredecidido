#ifndef BEHAVIOURENDEDCONDITION_H
#define BEHAVIOURENDEDCONDITION_H

#include "../EventCondition.h"

class EventBehaviour;

class BehaviourEndedCondition : public EventConditionTemplate<"BehaviourEnded"> {
private:
    EventBehaviour const* _behaviour;

    Entity* getEntity(sol::table const& params);
    Event* getEvent(Entity* entity, sol::table const& params);
    bool initBehaviour(Event* event, sol::table const& params);
public:
    BehaviourEndedCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~BehaviourEndedCondition() override;
};



#endif //BEHAVIOURENDEDCONDITION_H
