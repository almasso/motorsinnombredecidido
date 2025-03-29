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

    _currentBehaviour = _behaviours.size();
    return true;
}

Event::Event(Game* game, Scene* scene, Entity* entity) :
    _game(game),
    _scene(scene),
    _entity(entity),
    _condition(nullptr),
    _currentBehaviour(-1),
    _isPaused(true),
    _targetBehaviour(-1) {
}

bool Event::init(sol::table const& event) {
    if (!initCondition(event))
        return false;
    if (!initBehaviours(event))
        return false;
    return true;
}

void Event::updateEnd() {
    for (auto it = _pendingEnd.begin(); it != _pendingEnd.end();) {
        (*it)->updateEnd();
        if ((*it)->ended())
            it = _pendingEnd.erase(it);
        else ++it;
    }
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
    _currentBehaviour = 0;
    resume();
}

void Event::resume() {
    _isPaused = false;
}

void Event::pause() {
    _isPaused = true;
}

void Event::stop() {
    _currentBehaviour = _behaviours.size();
    pause();
}

void Event::jump(int index) {
    if (index < _behaviours.size())
        _targetBehaviour = index;
}

bool Event::update() {
    if (_targetBehaviour != -1) {
        _currentBehaviour = _targetBehaviour;
        _targetBehaviour = -1;
    }

    updateEnd();

    if (_currentBehaviour == _behaviours.size()) {
        if (_condition->met())
            start();
        else return true;
    }

    if (_isPaused)
        return true;


    auto behaviour = _behaviours[_currentBehaviour];
    if (!behaviour->act())
        return false;

    if (behaviour->done()) {
        ++_currentBehaviour;
        if (!behaviour->ended())
            _pendingEnd.push_back(behaviour);
    }
    return true;
}

void Event::RegisterToLua(sol::state& lua) {
    sol::usertype type = lua.new_usertype<Event>("Event");
    type["start"] = &Event::start;
    type["resume"] = &Event::resume;
    type["pause"] = &Event::pause;
    type["stop"] = &Event::stop;
    type["jump"] = &Event::jump;
}
