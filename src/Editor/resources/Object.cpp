//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Object.h"

#include <sol/table.hpp>

#include "events/Event.h"

#define localVarsKey "localVariables"
#define xKey "x"
#define yKey "y"
#define collidableKey "collidable"
#define eventsKey "events"

editor::resources::Object::Object() :
    _x(0),
    _y(0),
    _collidable(false) {
}

editor::resources::Object::~Object() {
    for (auto& event : _events) {
        delete event;
        event = nullptr;
    }
    _events.clear();
    _localVariables.clear();
}

bool editor::resources::Object::read(sol::table const& objectTable) {
    sol::optional<int> x = objectTable.get<sol::optional<int>>(xKey);
    if (!x.has_value())
        return false;
    _x = x.value();

    sol::optional<int> y = objectTable.get<sol::optional<int>>(yKey);
    if (!y.has_value())
        return false;
    _y = y.value();

    sol::optional<bool> collidable = objectTable.get<sol::optional<bool>>(collidableKey);
    if (!collidable.has_value())
        return false;
    _collidable = collidable.value();

    sol::optional<sol::table> events = objectTable.get<sol::optional<sol::table>>(eventsKey);
    if (!events.has_value())
        return false;
    if (!readEvents(*events))
        return false;

    sol::optional<sol::table> localVars = objectTable.get<sol::optional<sol::table>>(localVarsKey);
    if (!localVars.has_value())
        return false;
    if (!readLocalVars(*localVars))
        return false;
    return true;
}

bool editor::resources::Object::write(sol::table& objectTable) {
    objectTable[xKey] = _x;
    objectTable[yKey] = _y;
    objectTable[collidableKey] = _collidable;

    sol::table events;
    if (!writeEvents(events))
        return false;
    objectTable[eventsKey] = events;

    sol::table localVars;
    writeLocalVars(localVars);
    objectTable[localVarsKey] = localVars;

    return true;
}

bool editor::resources::Object::writeToEngine(sol::table& objectTable) {
    return true;
}

int editor::resources::Object::getX() const {
    return _x;
}

int editor::resources::Object::getY() const {
    return _y;
}

bool editor::resources::Object::readEvents(sol::table const& events) {
    for (auto&& [name, event] : events) {
        if (!name.is<std::string>() && !event.is<sol::table>())
            return false;
        auto ev = new events::Event();
        _events.push_back(ev);
        if (!ev->read(name.as<std::string>(), event.as<sol::table>()))
            return false;
    }
    return true;
}

bool editor::resources::Object::readLocalVars(sol::table const& localVars) {
    for (auto&& [key, value] : localVars) {
        if (!key.is<std::string>() || !value.is<sol::lua_value>())
            return false;
        _localVariables.insert({key.as<std::string>(), value.as<sol::lua_value>()});
    }
    return true;
}

bool editor::resources::Object::writeEvents(sol::table& events) {
    for (auto& event : _events) {
        sol::table eventTable;
        if (!event->write(eventTable))
            return false;
        events[event->getName()] = eventTable;
    }
    return true;
}

void editor::resources::Object::writeLocalVars(sol::table& localVars) {
    for (auto& [key, variable] : _localVariables) {
        localVars[key] = variable;
    }
}
