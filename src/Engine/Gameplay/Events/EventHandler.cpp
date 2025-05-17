#include "EventHandler.h"

#include <Core/ComponentData.h>
#include <Utils/Error.h>

#include "Event.h"

bool EventHandler::addEvent(std::string const& name, sol::table const& eventTable) {
    auto event = Event::Create(_game, _scene, _entity, eventTable);
    if (!event) {
        Error::ShowError("EventHandler", "Could not create event \"" + name + "\".");
        return false;
    }
    if (!_events.insert({name, event}).second) {
        delete event;
        Error::ShowError("EventHandler", "Event \"" + name + "\" already exists in this EventHandler.");
        return false;
    }

    return true;
}

EventHandler::EventHandler(ComponentData const* data) :
    ComponentTemplate(data) {
}

EventHandler::~EventHandler() {
    for (auto& [name, event] : _events) {
        delete event;
        event = nullptr;
    }
}

bool EventHandler::init() {
    auto events = _data->getData<sol::table>("events", sol::lua_nil);
    if (!events.valid()) {
        Error::ShowError("EventHandler", "Invalid events table");
        return false;
    }
    for (auto const& [name, event] : events) {
        if (!name.is<std::string>() || !event.is<sol::table>()) {
            Error::ShowError("EventHandler", "Invalid event name or table format.");
            return false;
        }
        if (!addEvent(name.as<std::string>(), event.as<sol::table>())){
            Error::ShowError("EventHandler", "Event \"" + name.as<std::string>() + "\" is invalid.");
            return false;
        }
    }
    return true;
}

bool EventHandler::update() {
    for (auto& [name, event] : _events) {
        if (!event->update()) {
            Error::ShowError("EventHandler", "Event \"" + name + "\" updating failed.");
            return false;
        }
    }
    return true;
}

Event* EventHandler::getEvent(std::string const& name) {
    auto it = _events.find(name);
    if (it == _events.end()) {
        Error::ShowError("EventHandler", "Event \"" + name + "\" not present in the EventHandler.");
        return nullptr;
    }
    return it->second;
}

void EventHandler::RegisterToLua(sol::state& luaState) {
    sol::usertype<EventHandler> type = luaState.new_usertype<EventHandler>("EventHandler");
    type["getEvent"] = &EventHandler::getEvent;
    type["get"] = EventHandler::get;
}
