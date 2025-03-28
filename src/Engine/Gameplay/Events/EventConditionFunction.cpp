#include "EventConditionFunction.h"

#include <Load/LuaReader.h>

#include "EventCondition.h"

bool OnStart::init(sol::table const& params) {
    alreadyMet = false;
    return true;
}

bool OnStart::met() {
    if (alreadyMet)
        return false;
    alreadyMet = true;
    return true;
}

void OnStart::release() {
}

bool Always::init(sol::table const& params) {
    return true;
}

bool Always::met() {
    return true;
}

void Always::release() {
}

bool AndCondition::init(sol::table const& params) {
    auto conditionA = LuaReader::GetTable(params, "conditionA");
    if (!conditionA.valid())
        return false;

    _conditionA = EventCondition::Create(conditionA);
    if (!_conditionA)
        return false;

    auto conditionB = LuaReader::GetTable(params, "conditionB");
    if (!conditionB.valid())
        return false;

    _conditionB = EventCondition::Create(conditionB);
    if (_conditionB)
        return true;

    delete _conditionA;
    _conditionA = nullptr;
    return false;
}

bool AndCondition::met() {
    return _conditionA->met() && _conditionB->met();
}

void AndCondition::release() {
    delete _conditionA;
    delete _conditionB;
}

bool OrCondition::init(sol::table const& params) {
    auto conditionA = LuaReader::GetTable(params, "conditionA");
    if (!conditionA.valid())
        return false;

    _conditionA = EventCondition::Create(conditionA);
    if (!_conditionA)
        return false;

    auto conditionB = LuaReader::GetTable(params, "conditionB");
    if (!conditionB.valid())
        return false;

    _conditionB = EventCondition::Create(conditionB);
    if (_conditionB)
        return true;

    delete _conditionA;
    _conditionA = nullptr;
    return false;
}

bool OrCondition::met() {
    return _conditionA->met() || _conditionB->met();
}

void OrCondition::release() {
    delete _conditionA;
    delete _conditionB;
}

bool CollidedWith::init(sol::table const& params) {
    return true;
}

bool CollidedWith::met() {
    return false;
}

void CollidedWith::release() {
}

bool ValueEquals::init(sol::table const& params) {
    return true;
}

bool ValueEquals::met() {
    return false;
}

void ValueEquals::release() {
}

bool BehaviourEnded::init(sol::table const& params) {
    return true;
}

bool BehaviourEnded::met() {
    return false;
}

void BehaviourEnded::release() {
}

bool TimePassed::init(sol::table const& params) {
    return true;
}

bool TimePassed::met() {
    return false;
}

void TimePassed::release() {
}

bool EventConditionFunction::init(EventConditionType type, sol::table const& params) {
    switch (type) {
    case ON_START:
        return onStart.init(params);
    case ALWAYS:
        return always.init(params);
    case AND:
        return andCondition.init(params);
    case OR:
        return orCondition.init(params);
    case COLLIDED_WITH:
        return collidedWith.init(params);
    case VALUE_EQUALS:
        return valueEquals.init(params);
    case BEHAVIOUR_ENDED:
        return behaviourEnded.init(params);
    case TIME_PASSED:
        return timePassed.init(params);
    default:
        return false;
    }
}

bool EventConditionFunction::met(EventConditionType type) {
    switch (type) {
    case ON_START:
        return onStart.met();
    case ALWAYS:
        return always.met();
    case AND:
        return andCondition.met();
    case OR:
        return orCondition.met();
    case COLLIDED_WITH:
        return collidedWith.met();
    case VALUE_EQUALS:
        return valueEquals.met();
    case BEHAVIOUR_ENDED:
        return behaviourEnded.met();
    case TIME_PASSED:
        return timePassed.met();
    default:
        return false;
    }
}

void EventConditionFunction::release(EventConditionType type) {
    switch (type) {
    case ON_START:
        return onStart.release();
    case ALWAYS:
        return always.release();
    case AND:
        return andCondition.release();
    case OR:
        return orCondition.release();
    case COLLIDED_WITH:
        return collidedWith.release();
    case VALUE_EQUALS:
        return valueEquals.release();
    case BEHAVIOUR_ENDED:
        return behaviourEnded.release();
    case TIME_PASSED:
        return timePassed.release();
    default:
        return;
    }
}
