#ifndef ANDCONDITION_H
#define ANDCONDITION_H

#include "../EventCondition.h"

class AndCondition : public EventConditionTemplate<"And"> {
private:
    EventCondition* _conditionA;
    EventCondition* _conditionB;
public:
    AndCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~AndCondition() override;
};



#endif //ANDCONDITION_H
