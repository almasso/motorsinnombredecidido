#ifndef EVENTCONDITION_H
#define EVENTCONDITION_H

#include "EventConditionType.h"
#include "EventConditionFunction.h"

class EventCondition {
private:
    EventConditionType _type;
    EventConditionFunction _function;

    bool getType(sol::table const& condition);
    bool getParams(sol::table const& condition);

    EventCondition();
    bool init(sol::table const& condition);

public:
    static EventCondition* Create(sol::table const& condition);
    ~EventCondition();

    bool met();
};



#endif //EVENTCONDITION_H
