#include "EventBehaviour.h"

#include <Load/LuaReader.h>
#include <Core/Game.h>
#include <Core/Scene.h>
#include <Core/Entity.h>
#include <Utils/Error.h>

#include "Event.h"

EventBehaviour::EventBehaviour(Game* game, Scene* scene, Entity* entity, Event* event) :
    _game(game),
    _scene(scene),
    _entity(entity),
    _event(event) {
}

bool EventBehaviour::init(sol::table const& behaviour) {
    _self = behaviour;

    _act = LuaReader::GetFunction(behaviour, "act");
    if (!_act.valid()) {
        Error::ShowError("EventBehaviour", "EventBehaviour table has no method \"act\".");
        return false;
    }

    _done = LuaReader::GetFunction(behaviour, "done");
    if (!_done.valid()) {
        Error::ShowError("EventBehaviour", "EventBehaviour table has no method \"done\".");
        return false;
    }

    _ended = LuaReader::GetFunction(behaviour, "ended");
    if (!_ended.valid()) {
        Error::ShowError("EventBehaviour", "EventBehaviour table has no method \"ended\".");
        return false;
    }

    _onStart = LuaReader::GetFunction(behaviour, "onStart");
    if (!_onStart.valid()) {
        Error::ShowError("EventBehaviour", "EventBehaviour table has no method \"onStart\".");
        return false;
    }

    _init = LuaReader::GetFunction(behaviour, "init");
    if (!_init.valid()) {
        Error::ShowError("EventBehaviour", "EventBehaviour table has no method \"init\".");
        return false;
    }

    return true;
}

EventBehaviour* EventBehaviour::Create(Game* game, Scene* scene, Entity* entity, Event* event,
    sol::table const& behaviour) {
    auto instance = new EventBehaviour(game, scene, entity, event);

    if (instance->init(behaviour))
        return instance;

    delete instance;
    return nullptr;
}

bool EventBehaviour::init() {
    return _init(_self, _scene, _entity, _event);
}

bool EventBehaviour::onStart() {
    return _onStart(_self);
}

bool EventBehaviour::act() {
    return _act(_self, _game, _scene, _entity, _event);
}

bool EventBehaviour::done() const {
    return _done(_self);
}

bool EventBehaviour::ended() const {
    return _ended(_self, _scene, _entity, _event);
}

void EventBehaviour::RegisterToLua(sol::state& lua) {
    sol::usertype<EventBehaviour> type = lua.new_usertype<EventBehaviour>("EventBehaviour");
    type["done"] = &EventBehaviour::done;
    type["ended"] = &EventBehaviour::ended;
}
