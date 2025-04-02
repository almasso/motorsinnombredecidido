#ifndef VALUEEQUALSCONDITION_H
#define VALUEEQUALSCONDITION_H

#include "../EventCondition.h"
#include <sol/lua_value.hpp>

class LocalVariables;

class ValueEqualsCondition : public EventConditionTemplate<"ValueEquals"> {
private:
    std::string _variable;
    sol::lua_value _equals;
    LocalVariables* _localVariables;

public:
    ValueEqualsCondition();
    bool init(sol::table const& params) override;
    bool met() override;
    ~ValueEqualsCondition() override;
};



#endif //VALUEEQUALSCONDITION_H
