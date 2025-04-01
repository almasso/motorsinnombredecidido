#include "EventCondition.h"

#include <sol/state.hpp>

EventCondition::EventCondition() :
    _scene(nullptr),
    _entity(nullptr),
    _event(nullptr) {
}

void EventCondition::setContext(Scene* scene, Entity* entity, Event* event) {
    _scene = scene;
    _entity = entity;
    _event = event;
}

void EventCondition::reset() {
}

EventCondition::~EventCondition() = default;

void EventCondition::RegisterToLua(sol::state& luaState) {
    sol::usertype<EventCondition> type = luaState.new_usertype<EventCondition>("EventCondition");
    type["met"] = &EventCondition::met;
    type["reset"] = &EventCondition::reset;
}
