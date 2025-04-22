//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "AnimationBehaviour.h"

#define actionKey "action"
#define animationChangeKey "animation"

editor::resources::events::AnimationBehaviour::AnimationBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _action(PLAY),
    _animationToChange() {
}

editor::resources::events::AnimationBehaviour::~AnimationBehaviour() = default;

bool editor::resources::events::AnimationBehaviour::read(sol::table const& params) {
    std::string action = params.get_or<std::string>(actionKey, "");
    if (action.empty())
        return false;
    if (action == "play")
        _action = PLAY;
    else if (action == "stop")
        _action = STOP;
    else if (action == "reset")
        _action = RESET;
    else if (action == "change") {
        _action = CHANGE;
        _animationToChange = params.get_or<std::string>(animationChangeKey, "");
        if (_animationToChange.empty())
            return false;
    }
    else return false;

    return true;
}

bool editor::resources::events::AnimationBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::AnimationBehaviour::render() {
    return false;
}

bool editor::resources::events::AnimationBehaviour::writeParams(sol::table& params) {
    switch (_action) {
    case PLAY:
        params[actionKey] = "play";
        break;
    case STOP:
        params[actionKey] = "stop";
        break;
    case RESET:
        params[actionKey] = "reset";
        break;
    case CHANGE:
        params[actionKey] = "change";
        params[animationChangeKey] = _animationToChange;
        break;
    }
    return true;
}
