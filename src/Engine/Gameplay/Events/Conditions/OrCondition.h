#ifndef ORCONDITION_H
#define ORCONDITION_H

#include "../EventCondition.h"

class OrCondition : public EventConditionTemplate<"Or"> {
private:
    EventCondition* _conditionA;
    EventCondition* _conditionB;
public:
    OrCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~OrCondition() override;
};



#endif //ORCONDITION_H
