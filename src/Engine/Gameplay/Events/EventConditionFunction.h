#ifndef EVENTCONDITIONFUNCTION_H
#define EVENTCONDITIONFUNCTION_H

#include <sol/forward.hpp>

#include "EventConditionType.h"

class EventCondition;

class OnStart {
private:
    bool alreadyMet;
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};
class Always {
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};
class AndCondition {
private:
    EventCondition* _conditionA;
    EventCondition* _conditionB;
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};

class OrCondition {
private:
    EventCondition* _conditionA;
    EventCondition* _conditionB;
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};

class CollidedWith {
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};
class ValueEquals {
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};
class BehaviourEnded {
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};
class TimePassed {
public:
    bool init(sol::table const& params);
    bool met();
    void release();
};

union EventConditionFunction {
    OnStart onStart;
    Always always;
    AndCondition andCondition;
    OrCondition orCondition;
    CollidedWith collidedWith;
    ValueEquals valueEquals;
    BehaviourEnded behaviourEnded;
    TimePassed timePassed;

    bool init(EventConditionType type, sol::table const& params);
    bool met(EventConditionType type);
    void release(EventConditionType type);
};



#endif //EVENTCONDITIONFUNCTION_H
