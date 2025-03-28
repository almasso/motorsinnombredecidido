#include "EventCondition.h"
#include <string>
#include <Load/LuaReader.h>
#include <sol/sol.hpp>

static const std::unordered_map<std::string, EventConditionType> conditionToEnum {
            {"OnStart", ON_START},
            {"Always", ALWAYS},
            {"And", AND},
            {"Or", OR},
            {"CollidedWith", COLLIDED_WITH},
            {"ValueEquals", VALUE_EQUALS},
            {"BehaviourEnded", BEHAVIOUR_ENDED},
            {"TimePassed", TIME_PASSED}
};

bool EventCondition::getType(sol::table const& condition) {
    std::string typeName = condition.get_or<std::string>("type", "");
    if (typeName.empty())
        return false;

    auto typeIt = conditionToEnum.find(typeName);
    if (typeIt == conditionToEnum.end())
        return false;

    _type = typeIt->second;
    return true;
}

bool EventCondition::getParams(sol::table const& condition) {
    auto paramsTable = LuaReader::GetTable(condition, "params");
    return _function.init(_type, paramsTable);
}

EventCondition::EventCondition() :
    _type(NONE),
    _function() {
}

bool EventCondition::init(sol::table const& condition) {
    if (!getType(condition))
        return false;
    if (!getParams(condition))
        return false;
    return true;
}

EventCondition* EventCondition::Create(sol::table const& condition) {
    auto instance = new EventCondition();

    if (instance->init(condition))
        return instance;

    delete instance;
    return nullptr;
}

EventCondition::~EventCondition() {
    _function.release(_type);
}

bool EventCondition::met() {
    return _function.met(_type);
}
