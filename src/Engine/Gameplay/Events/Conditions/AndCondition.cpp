#include "AndCondition.h"

#include <Load/LuaReader.h>

#include "../EventConditionFactory.h"

AndCondition::AndCondition() :
    _conditionA(nullptr),
    _conditionB(nullptr) {
}

bool AndCondition::init(sol::table const& params) {
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

bool AndCondition::met() {
    bool a = _conditionA->met();
    bool b = _conditionB->met();
    return a && b;
}

AndCondition::~AndCondition() {
    delete _conditionA;
    delete _conditionB;
}