#include "OrCondition.h"

#include <Load/LuaReader.h>

#include "../EventConditionFactory.h"

OrCondition::OrCondition() :
    _conditionA(nullptr),
    _conditionB(nullptr) {
}

bool OrCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;

    auto conditionA = LuaReader::GetTable(params, "conditionA");
    if (!conditionA.valid())
        return false;

    _conditionA = EventConditionFactory::Create(conditionA);
    if (!_conditionA)
        return false;

    auto conditionB = LuaReader::GetTable(params, "conditionB");
    if (!conditionB.valid())
        return false;

    _conditionB = EventConditionFactory::Create(conditionB);
    if (_conditionB)
        return true;

    delete _conditionA;
    _conditionA = nullptr;
    return false;
}

bool OrCondition::met() {
    bool a = _conditionA->met();
    bool b = _conditionB->met();
    return a || b;
}

OrCondition::~OrCondition() {
    delete _conditionA;
    delete _conditionB;
}
