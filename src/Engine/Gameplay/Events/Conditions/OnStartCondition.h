#ifndef ONSTARTCONDITION_H
#define ONSTARTCONDITION_H

#include "../EventCondition.h"

class OnStartCondition : public EventConditionTemplate<"OnStart"> {
private:
    bool alreadyMet;
public:
    OnStartCondition();
    bool init(sol::table const& params) override;
    void reset() override;
    bool met() override;
    ~OnStartCondition() override;
};



#endif //ONSTARTCONDITION_H
