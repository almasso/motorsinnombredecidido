#ifndef EVENTCONDITIONTYPE_H
#define EVENTCONDITIONTYPE_H

enum EventConditionType {
    NONE = -1,
    ON_START,
    ALWAYS,
    AND,
    OR,
    COLLIDED_WITH,
    VALUE_EQUALS,
    BEHAVIOUR_ENDED,
    TIME_PASSED
};

#endif //EVENTCONDITIONTYPE_H
