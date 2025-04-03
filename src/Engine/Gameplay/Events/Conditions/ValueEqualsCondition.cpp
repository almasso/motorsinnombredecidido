#include "ValueEqualsCondition.h"
#include <Gameplay/Events/LocalVariables.h>
#include <Core/Entity.h>
#include <sol/table.hpp>

ValueEqualsCondition::ValueEqualsCondition() :
    _variable(),
    _equals(sol::lua_nil) {
}

bool ValueEqualsCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;
    _variable = params.get_or<std::string>("variable", "");
    if (_variable.empty())
        return false;
    _equals = params.get_or<sol::lua_value>("equals", sol::lua_nil);
    if (_equals.value() == sol::lua_nil)
        return false;
    _localVariables = _entity->getComponent<LocalVariables>();
    return _localVariables != nullptr;
}

bool ValueEqualsCondition::met() {
    return _equals.value() == _localVariables->getVariable(_variable).value();
}

ValueEqualsCondition::~ValueEqualsCondition() {
    _variable = "";
    _equals = sol::lua_nil;
    _localVariables = nullptr;
}
