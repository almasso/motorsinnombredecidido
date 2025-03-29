#include "EventHandler.h"

#include <Core/ComponentData.h>
#include <Utils/RPGError.h>

#include "Event.h"

bool EventHandler::addEvent(std::string const& name, sol::table const& eventTable) {
    auto event = Event::Create(_game, _scene, _entity, eventTable);
    if (!event)
        return false;

    if (!_events.insert({name, event}).second) {
        delete event;
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
        RPGError::ShowError("EventHandler", "Invalid events table");
        return false;
    }
    for (auto const& [name, event] : events) {
        if (!name.is<std::string>() || !event.is<sol::table>()) {
            RPGError::ShowError("EventHandler", "Invalid event name or table format");
            return false;
        }
        if (!addEvent(name.as<std::string>(), event.as<sol::table>())){
            RPGError::ShowError("EventHandler", "Invalid event table");
            return false;
        }
    }
    return true;
}

bool EventHandler::update() {
    for (auto& [name, event] : _events) {
        event->update();
    }
    return true;
}
