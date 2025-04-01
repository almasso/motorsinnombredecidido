#include "NotCondition.h"

#include "../EventConditionFactory.h"
#include <Load/LuaReader.h>

NotCondition::NotCondition() :
    _condition(nullptr) {
}

bool NotCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;

    auto condition = LuaReader::GetTable(params, "condition");
    if (!condition.valid())
        return false;

    _condition = EventConditionFactory::Create(condition, _scene, _entity, _event);
    return _condition != nullptr;
}

bool NotCondition::met() {
    return !_condition->met();
}

NotCondition::~NotCondition() {
    delete _condition;
}