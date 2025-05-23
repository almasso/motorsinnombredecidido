#include "Event.h"

#include <Load/LuaReader.h>
#include <Utils/Error.h>

#include "EventBehaviour.h"
#include "EventCondition.h"
#include "EventConditionFactory.h"

bool Event::initCondition(sol::table const& event) {
    auto condition = LuaReader::GetTable(event, "condition");
    if (!condition.valid())
        return false;
    _condition = EventConditionFactory::Create(condition, _scene, _entity, this);
    if (!_condition)
        return false;
    return true;
}

bool Event::insertBehaviour(sol::table const& behaviour) {
    auto eventBehaviour = EventBehaviour::Create(_game, _scene, _entity, this, behaviour);
    if (!eventBehaviour) {
        Error::ShowError("Failed creating EventBehaviour", "Something went wrong when trying to create an EventBehaviour");
        return false;
    }
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
    _loop(false),
    _isPaused(true),
    _targetBehaviour(-1) {
}

bool Event::init(sol::table const& event) {
    _loop = event.get_or("loop", false);
    if (!initCondition(event)) {
        Error::ShowError("Event", "Could not create EventCondition.");
        return false;
    }
    if (!initBehaviours(event))
        return false;
    for (auto& behaviour : _behaviours) {
        if (!behaviour->init())
            return false;
    }
    _condition->reset();
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
    _currentBehaviour = 0;
    _behaviours[_currentBehaviour]->onStart();
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

EventBehaviour const* Event::getBehaviour(int index) const {
    if (index >= _behaviours.size() || index < 0) {
        Error::ShowError("Event", "Can't access EventBehaviour with index " + std::to_string(index) + ".");
        return nullptr;
    }
    return _behaviours[index];
}

bool Event::update() {
    if (_targetBehaviour != -1) {
        _currentBehaviour = _targetBehaviour;
        _behaviours[_currentBehaviour]->onStart();
        _targetBehaviour = -1;
    }

    if (!_behaviours.empty() && _currentBehaviour == _behaviours.size()) {
        if (_condition->met())
            start();
        else return true;
    }

    if (_isPaused)
        return true;

    auto behaviour = _behaviours[_currentBehaviour];
    if (!behaviour->act()) {
        Error::ShowError("Event", "Failed to do EventBehaviour action.");
        return false;
    }

    if (behaviour->done()) {
        ++_currentBehaviour;
        if (_currentBehaviour < _behaviours.size())
            _behaviours[_currentBehaviour]->onStart();
        else if (_loop) {
            _currentBehaviour = 0;
            _behaviours[_currentBehaviour]->onStart();
        }
    }
    return true;
}

void Event::RegisterToLua(sol::state& lua) {
    sol::usertype<Event> type = lua.new_usertype<Event>("Event");
    type["start"] = &Event::start;
    type["resume"] = &Event::resume;
    type["pause"] = &Event::pause;
    type["stop"] = &Event::stop;
    type["jump"] = &Event::jump;
    type["getBehaviour"] = &Event::getBehaviour;
}
