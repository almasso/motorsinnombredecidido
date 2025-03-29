#include "Event.h"

#include <Load/LuaReader.h>

#include "EventBehaviour.h"
#include "EventCondition.h"
#include "EventConditionFactory.h"

bool Event::initCondition(sol::table const& event) {
    auto condition = LuaReader::GetTable(event, "condition");
    if (!condition.valid())
        return false;
    _condition = EventConditionFactory::Create(condition);
    return _condition != nullptr;
}

bool Event::insertBehaviour(sol::table const& behaviour) {
    auto eventBehaviour = EventBehaviour::Create(_game, _scene, _entity, this, behaviour);
    if (!eventBehaviour)
        return false;
    _behaviours.push_back(eventBehaviour);
    return true;
}

bool Event::initBehaviours(sol::table const& event) {
    auto behaviours = LuaReader::GetTable(event, "behaviours");
    if (!behaviours.valid())
        return false;

    for (auto const& [key, behaviour] : behaviours) {
        if (!behaviour.is<sol::table>())
            return false;
        if (!insertBehaviour(behaviour))
            return false;
    }

    _currentBehaviour = _behaviours.end();
    return true;
}

Event::Event(Game* game, Scene* scene, Entity* entity) :
    _game(game),
    _scene(scene),
    _entity(entity),
    _condition(nullptr),
    _isPaused(true) {
}

bool Event::init(sol::table const& event) {
    if (!initCondition(event))
        return false;
    if (!initBehaviours(event))
        return false;
    return true;
}

Event* Event::Create(Game* game, Scene* scene, Entity* entity, sol::table const& event) {
    auto instance = new Event(game, scene, entity);

    if (instance->init(event))
        return instance;

    delete instance;
    return nullptr;
}

Event::~Event() {
    delete _condition;
    for (auto& behaviour : _behaviours) {
        delete behaviour;
        behaviour = nullptr;
    }
}

void Event::start() {
    _currentBehaviour = _behaviours.begin();
    resume();
}

void Event::resume() {
    _isPaused = false;
}

void Event::update() {
    if (_currentBehaviour == _behaviours.end())
        if (_condition->met())
            start();
        else
            return;

    if (_isPaused)
        return;
    auto behaviour = *_currentBehaviour;
    behaviour->act();
    if (behaviour->done())
        ++_currentBehaviour;
}

void Event::pause() {
    _isPaused = true;
}

void Event::stop() {
    _currentBehaviour = _behaviours.end();
    pause();
}
