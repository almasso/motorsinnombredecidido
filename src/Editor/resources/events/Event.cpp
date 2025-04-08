//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Event.h"

#include <io/LuaManager.h>
#include <sol/table.hpp>
#include "EventCondition.h"
#include "EventBehaviour.h"
#include "EventBehaviourFactory.h"
#include "EventConditionFactory.h"

#define behavioursKey "behaviours"
#define conditionKey "condition"

editor::resources::events::Event::~Event() {
    delete _condition;
    _condition = nullptr;
    for (auto& behaviour : _behaviours) {
        delete behaviour;
        behaviour = nullptr;
    }
    _behaviours.clear();
}

void editor::resources::events::Event::init(std::string const& name) {
    _name = name;
}

bool editor::resources::events::Event::read(std::string const& name, sol::table const& eventTable) {
    init(name);

    sol::optional<sol::table> condition = eventTable.get<sol::optional<sol::table>>(conditionKey);
    if (!condition.has_value())
        return false;
    _condition = EventConditionFactory::Create(*condition);
    if (!_condition)
        return false;

    sol::optional<sol::table> behaviours = eventTable.get<sol::optional<sol::table>>(behavioursKey);
    if (!behaviours.has_value())
        return false;
    if (!readBehaviours(*behaviours))
        return false;

    return true;
}

bool editor::resources::events::Event::write(sol::table& eventTable) {
    sol::table conditionTable = io::LuaManager::GetInstance().getState().create_table();
    if (!_condition->write(conditionTable))
        return false;
    eventTable[conditionKey] = conditionTable;

    sol::table behavioursTable = io::LuaManager::GetInstance().getState().create_table();
    if (!writeBehaviours(behavioursTable))
        return false;
    eventTable[behavioursKey] = behavioursTable;

    return true;
}

std::string const& editor::resources::events::Event::getName() const {
    return _name;
}

editor::resources::events::EventBehaviour* editor::resources::events::Event::addBehaviour(std::string const& id) {
    auto behaviour = EventBehaviourFactory::Create(id);
    _behaviours.push_back(behaviour);
    return behaviour;
}

bool editor::resources::events::Event::readBehaviours(sol::table const& behaviours) {
    for (auto&& [key, behaviour] : behaviours) {
        if (!behaviours.is<sol::table>())
            return false;
        auto beh = EventBehaviourFactory::Create(behaviour.as<sol::table>());
        if (!beh)
            return false;
        _behaviours.push_back(beh);
    }
    return true;
}

bool editor::resources::events::Event::writeBehaviours(sol::table& behaviours) {
    for (auto& behaviour : _behaviours) {
        sol::table behaviourTable = io::LuaManager::GetInstance().getState().create_table();
        if (!behaviour->write(behaviourTable))
            return false;
        behaviours.add(behaviourTable);
    }
    return true;
}
