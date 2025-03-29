#include "EventBehaviour.h"

#include <Load/LuaReader.h>
#include <Core/Game.h>
#include <Core/Scene.h>
#include <Core/Entity.h>
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
    if (!_act.valid())
        return false;

    _done = LuaReader::GetFunction(behaviour, "done");
    if (!_done.valid())
        return false;

    _ended = LuaReader::GetFunction(behaviour, "ended");
    if (!_ended.valid())
        return false;

    _updateEnd = LuaReader::GetFunction(behaviour, "updateEnd");
    if (!_updateEnd.valid())
        return false;

    auto init = LuaReader::GetFunction(behaviour, "init");
    if (!init.valid())
        return false;
    if (!init(_self, _entity))
        return false;

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

bool EventBehaviour::act() {
    return _act(_self, _game, _scene, _entity, _event);
}

bool EventBehaviour::done() const {
    return _done(_self);
}

bool EventBehaviour::ended() const {
    return _ended(_self);
}

void EventBehaviour::updateEnd() {
    _updateEnd(_self, _scene, _entity, _event);
}
