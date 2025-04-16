//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MusicBehaviour.h"

#define actionKey "action"
#define clipKey "clip"
#define volumeKey "volume"
#define loopKey "loop"

editor::resources::events::MusicBehaviour::MusicBehaviour() :
    _action(PLAY),
    _param() {
}

editor::resources::events::MusicBehaviour::~MusicBehaviour() = default;

bool editor::resources::events::MusicBehaviour::read(sol::table const& params) {
    std::string action = params.get_or<std::string>(actionKey, "");
    if (action == "play")
        _action = PLAY;
    else if (action == "stop")
        _action = STOP;
    else if (action == "resume")
        _action = RESUME;
    else if (action == "pause")
        _action = PAUSE;
    else if (action == "change") {
        _action = CHANGE;
        std::string clip = params.get<std::string>(clipKey, "");
        if (clip.empty())
            return false;
        _param.clip = clip.c_str();
    }
    else if (action == "volume") {
        _action = VOLUME;
        sol::optional<float> volume = params.get<sol::optional<float>>(volumeKey);
        if (!volume.has_value())
            return false;
        _param.volume = volume.value();
    }
    else if (action == "loop") {
        _action = LOOP;
        sol::optional<bool> loop = params.get<sol::optional<bool>>(loopKey);
        if (!loop.has_value())
            return false;
        _param.loop = loop.value();
    }
    else return false;
    return true;
}

bool editor::resources::events::MusicBehaviour::writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) {
    return true;
}

bool editor::resources::events::MusicBehaviour::writeParams(sol::table& params) {
    switch (_action) {
    case PLAY:
        params[actionKey] = "play";
        break;
    case STOP:
        params[actionKey] = "stop";
        break;
    case RESUME:
        params[actionKey] = "resume";
        break;
    case PAUSE:
        params[actionKey] = "pause";
        break;
    case CHANGE:
        params[actionKey] = "change";
        params[clipKey] = _param.clip;
        break;
    case VOLUME:
        params[actionKey] = "volume";
        params[volumeKey] = _param.volume;
        break;
    case LOOP:
        params[actionKey] = "loop";
        params[loopKey] = _param.loop;
        break;
    }
    return true;
}
