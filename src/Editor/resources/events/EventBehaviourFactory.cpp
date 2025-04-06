//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventBehaviourFactory.h"
#include "EventBehaviour.h"
#include "behaviours/MoveBehaviour.h"

std::unordered_map <std::string, std::function<editor::resources::events::EventBehaviour*()>> editor::resources::events::EventBehaviourFactory::_behaviours;

editor::resources::events::EventBehaviour* editor::resources::events::EventBehaviourFactory::Create(std::string const& id) {
    auto it = _behaviours.find(id);
    if (it == _behaviours.end())
        return nullptr;
    return it->second();
}

editor::resources::events::EventBehaviour* editor::resources::events::EventBehaviourFactory::Create(sol::table const& behaviour) {
    std::string id = behaviour.get_or<std::string>(idKey, "");
    if (id.empty())
        return nullptr;

    sol::optional<sol::table> params = behaviour.get<sol::optional<sol::table>>(paramsKey);
    if (!params.has_value())
        return nullptr;

    auto instance = Create(id);
    if (!instance)
        return nullptr;

    if (instance->read(*params))
        return instance;

    delete instance;
    return nullptr;
}

void editor::resources::events::EventBehaviourFactory::init() {
    RegisterBehaviour<MoveBehaviour>();
}

