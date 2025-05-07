//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventBehaviourFactory.h"
#include "EventBehaviour.h"
#include "behaviours/AnimationBehaviour.h"
#include "behaviours/ChoicesBehaviour.h"
#include "behaviours/DialogueBehaviour.h"
#include "behaviours/JumpBehaviour.h"
#include "behaviours/JumpIfBehaviour.h"
#include "behaviours/ModifyVariableBehaviour.h"
#include "behaviours/MoveBehaviour.h"
#include "behaviours/MusicBehaviour.h"
#include "behaviours/PlaySFXBehaviour.h"
#include "behaviours/WaitForBehaviour.h"

std::unordered_map <std::string, std::function<editor::resources::events::EventBehaviour*(editor::resources::events::Event* event)>> editor::resources::events::EventBehaviourFactory::_behaviours;
std::set<std::string> editor::resources::events::EventBehaviourFactory::_behaviourKeys;
bool editor::resources::events::EventBehaviourFactory::_initialized = false;

editor::resources::events::EventBehaviour* editor::resources::events::EventBehaviourFactory::Create(std::string const& id, Event* event) {
    if (!_initialized)
        Init();

    auto it = _behaviours.find(id);
    if (it == _behaviours.end())
        return nullptr;
    return it->second(event);
}

editor::resources::events::EventBehaviour* editor::resources::events::EventBehaviourFactory::Create(sol::table const& behaviour, Event* event) {
    std::string id = behaviour.get_or<std::string>(idKey, "");
    if (id.empty())
        return nullptr;

    sol::optional<sol::table> params = behaviour.get<sol::optional<sol::table>>(paramsKey);
    if (!params.has_value())
        return nullptr;

    auto instance = Create(id, event);
    if (!instance)
        return nullptr;

    if (instance->read(*params))
        return instance;

    delete instance;
    return nullptr;
}

std::set<std::string> const& editor::resources::events::EventBehaviourFactory::GetKeys() {
    return _behaviourKeys;
}

void editor::resources::events::EventBehaviourFactory::Init() {
    _initialized = true;
    RegisterBehaviour<AnimationBehaviour>();
    RegisterBehaviour<ChoicesBehaviour>();
    RegisterBehaviour<DialogueBehaviour>();
    RegisterBehaviour<JumpBehaviour>();
    RegisterBehaviour<JumpIfBehaviour>();
    RegisterBehaviour<ModifyVariableBehaviour>();
    RegisterBehaviour<MoveBehaviour>();
    RegisterBehaviour<MusicBehaviour>();
    RegisterBehaviour<PlaySFXBehaviour>();
    RegisterBehaviour<WaitForBehaviour>();
}

