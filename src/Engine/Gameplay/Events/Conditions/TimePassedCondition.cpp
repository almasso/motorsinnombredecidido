#include "TimePassedCondition.h"

#include <sol/table.hpp>
#include <Utils/Time.h>

TimePassedCondition::TimePassedCondition() :
    _timeToPass(0.0f),
    _startTime(-1.0f) {
}

bool TimePassedCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;
    _timeToPass = params.get_or("seconds", 0.0f);
    return true;
}

void TimePassedCondition::reset() {
    _startTime = Time::time;
}

bool TimePassedCondition::met() {
    return _timeToPass <= Time::time - _startTime;
}

TimePassedCondition::~TimePassedCondition() = default;

