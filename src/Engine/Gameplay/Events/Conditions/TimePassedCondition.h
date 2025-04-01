#ifndef TIMEPASSEDCONDITION_H
#define TIMEPASSEDCONDITION_H

#include "../EventCondition.h"

class TimePassedCondition : public EventConditionTemplate<"TimePassed"> {
private:
    float _timeToPass;
    float _startTime;
public:
    TimePassedCondition();
    bool init(sol::table const& params) override;
    void reset() override;
    bool met() override;
    ~TimePassedCondition() override;
};



#endif //TIMEPASSEDCONDITION_H
