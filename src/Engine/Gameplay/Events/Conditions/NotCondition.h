#ifndef NOTCONDITION_H
#define NOTCONDITION_H

#include "../EventCondition.h"

class NotCondition : public EventConditionTemplate<"Not"> {
private:
    EventCondition* _condition;
public:
    NotCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~NotCondition() override;
};



#endif //NOTCONDITION_H
