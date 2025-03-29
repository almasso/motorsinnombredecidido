#ifndef ALWAYSCONDITION_H
#define ALWAYSCONDITION_H

#include "../EventCondition.h"

class AlwaysCondition : public EventConditionTemplate<"Always"> {
public:
    bool init(sol::table const& params) override;
    bool met() override;
};



#endif //ALWAYSCONDITION_H
