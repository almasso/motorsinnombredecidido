#include "BehaviourEndedCondition.h"

#include <Core/Entity.h>
#include <Core/Scene.h>
#include <Gameplay/Events/Event.h>
#include <Gameplay/Events/EventBehaviour.h>
#include <Gameplay/Events/EventHandler.h>

#include <sol/table.hpp>

Entity* BehaviourEndedCondition::getEntity(sol::table const& params) {
    std::string entityHandler = params.get_or<std::string>("entity", "");
    if (entityHandler.empty())
        return _entity;
    return _scene->getEntityByHandler(entityHandler);
}

Event* BehaviourEndedCondition::getEvent(Entity* entity, sol::table const& params) {
    std::string eventId = params.get_or<std::string>("event", "");
    if (eventId.empty())
        return _event;
    auto eventHandler = entity->getComponent<EventHandler>();
    if (eventHandler == nullptr)
        return nullptr;
    return eventHandler->getEvent(eventId);
}

bool BehaviourEndedCondition::initBehaviour(Event* event, sol::table const& params) {
    int index = params.get_or<int, std::string, int>("behaviour", -1);
    _behaviour = event->getBehaviour(index);
    return _behaviour != nullptr;
}

BehaviourEndedCondition::BehaviourEndedCondition() :
    _behaviour(nullptr) {
}

bool BehaviourEndedCondition::init(sol::table const& params) {
    if (!params.valid())
        return false;
    Entity* entity = getEntity(params);
    if (entity == nullptr)
        return false;
    Event* event = getEvent(entity, params);
    if (event == nullptr)
        return false;
    return initBehaviour(event, params);
}

bool BehaviourEndedCondition::met() {
    return _behaviour->ended();
}

BehaviourEndedCondition::~BehaviourEndedCondition() = default;
