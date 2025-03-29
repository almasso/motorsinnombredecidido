#ifndef EVENTCONDITION_H
#define EVENTCONDITION_H

#include <sol/forward.hpp>

class EventCondition {
public:
    virtual bool init(sol::table const& params) = 0;
    virtual bool met() = 0;
    virtual ~EventCondition();
};

#include <Utils/string_literal.h>

template<string_literal conditionName>
class EventConditionTemplate : public EventCondition {
public:
    static constexpr const char* id = conditionName.value;
};


#endif //EVENTCONDITION_H
