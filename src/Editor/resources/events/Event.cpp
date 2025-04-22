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

std::filesystem::path editor::resources::events::Event::_eventsDirectory;

editor::resources::events::Event::Event(Project* project) :
    _project(project),
    _condition(nullptr),
    _initialized(false) {
}

editor::resources::events::Event::~Event() {
    delete _condition;
    _condition = nullptr;
    for (auto& behaviour : _behaviours) {
        delete behaviour;
        behaviour = nullptr;
    }
    _behaviours.clear();
}

bool editor::resources::events::Event::readFromLua(std::string const& name) {
    sol::table eventTable = io::LuaManager::GetInstance().getTable(GetFilePath(name));
    if (!eventTable.valid())
        return false;
    return read(name, eventTable);
}

bool editor::resources::events::Event::writeToLua() {
    sol::table event = io::LuaManager::GetInstance().getState().create_table();
    if (!write(event))
        return false;

    io::LuaManager::GetInstance().writeToFile(event, GetFilePath(_name));
    return true;
}

bool editor::resources::events::Event::eraseFromLua() {
    return EraseFromLua(_name);
}

bool editor::resources::events::Event::EraseFromLua(std::string const& name) {
    return std::filesystem::remove(GetFilePath(name));
}

void editor::resources::events::Event::init(std::string const& name) {
    _name = name;
    _initialized = true;
}

void editor::resources::events::Event::init(std::string const& name, std::string const& condition) {
    init(name);
    setCondition(condition);
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
    if (_condition && !_condition->write(conditionTable))
        return false;
    eventTable[conditionKey] = conditionTable;

    sol::table behavioursTable = io::LuaManager::GetInstance().getState().create_table();
    if (!writeBehaviours(behavioursTable))
        return false;
    eventTable[behavioursKey] = behavioursTable;

    return true;
}

bool editor::resources::events::Event::writeToEngine(sol::table& eventTable, std::vector<std::string>& componentDependencies) {
    return true;
}

std::string const& editor::resources::events::Event::getName() const {
    return _name;
}

editor::resources::events::EventBehaviour* editor::resources::events::Event::addBehaviour(std::string const& id) {
    auto behaviour = EventBehaviourFactory::Create(id, this);
    _behaviours.push_back(behaviour);
    return behaviour;
}

bool editor::resources::events::Event::isInitialized() const {
    return _initialized;
}

editor::resources::events::EventCondition* editor::resources::events::Event::getCondition() {
    return _condition;
}

void editor::resources::events::Event::setCondition(std::string const& condition) {
    delete _condition;
    _condition = EventConditionFactory::Create(condition);
}

std::list<editor::resources::events::EventBehaviour*>& editor::resources::events::Event::getBehaviours() {
    return _behaviours;
}

void editor::resources::events::Event::changeBehaviour(EventBehaviour* previousBehaviour, std::string const& newBehaviour) {
    for (auto& behaviour : _behaviours) {
        if (behaviour == previousBehaviour) {
            delete behaviour;
            behaviour = EventBehaviourFactory::Create(newBehaviour, this);
            return;
        }
    }
}

void editor::resources::events::Event::removeBehaviour(std::list<resources::events::EventBehaviour*>::iterator& behaviour) {
    behaviour = _behaviours.erase(behaviour);
}

void editor::resources::events::Event::moveBehaviourUp(std::list<EventBehaviour*>::iterator& behaviour) {
    _behaviours.splice(behaviour, _behaviours, behaviour--);
}

void editor::resources::events::Event::moveBehaviourDown(std::list<EventBehaviour*>::iterator& behaviour) {
    _behaviours.splice(++behaviour, _behaviours, behaviour++);
}

editor::Project* editor::resources::events::Event::getProject() {
    return _project;
}

void editor::resources::events::Event::SetEventsDirectory(std::filesystem::path const& eventsDirectory) {
    _eventsDirectory = eventsDirectory;
}

bool editor::resources::events::Event::readBehaviours(sol::table const& behaviours) {
    for (auto&& [key, behaviour] : behaviours) {
        if (!behaviours.is<sol::table>())
            return false;
        auto beh = EventBehaviourFactory::Create(behaviour.as<sol::table>(), this);
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

std::string editor::resources::events::Event::GetFilePath(std::string const& eventName) {
    return (_eventsDirectory / (eventName + ".lua")).string();
}