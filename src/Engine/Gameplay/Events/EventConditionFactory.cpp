#include "EventConditionFactory.h"

#include <Load/LuaReader.h>

#include "Conditions/AlwaysCondition.h"
#include "Conditions/AndCondition.h"
#include "Conditions/BehaviourEndedCondition.h"
#include "Conditions/OnStartCondition.h"
#include "Conditions/OrCondition.h"
#include "Conditions/TimePassedCondition.h"

std::unordered_map<std::string, std::function<EventCondition*()>> EventConditionFactory::_factory;

void EventConditionFactory::Init() {
    RegisterCondition<AndCondition>();
    RegisterCondition<OrCondition>();
    RegisterCondition<OnStartCondition>();
    RegisterCondition<AlwaysCondition>();
    RegisterCondition<BehaviourEndedCondition>();
    RegisterCondition<TimePassedCondition>();
}

bool EventConditionFactory::ReadCondition(sol::table const& condition, std::string& type, sol::table& params) {
    type = condition.get_or<std::string>("type", "");
    if (type.empty())
        return false;

    params = LuaReader::GetTable(condition, "params");
    return true;
}

EventCondition* EventConditionFactory::CreateCondition(std::string const& type, sol::table const& params) {
    auto it = _factory.find(type);
    if (it == _factory.end())
        return nullptr;

    auto instance = it->second();
    if (instance->init(params))
        return instance;

    delete instance;
    return nullptr;
}

EventCondition* EventConditionFactory::Create(sol::table const& condition) {
    if (_factory.empty())
        Init();

    std::string type;
    sol::table params;
    if (!ReadCondition(condition, type, params))
        return nullptr;

    return CreateCondition(type, params);
}

void EventConditionFactory::RegisterToLua(sol::state& luaState) {
    sol::usertype<EventConditionFactory> type = luaState.new_usertype<EventConditionFactory>("EventConditionFactory");
    type["Create"] = EventConditionFactory::Create;
}
