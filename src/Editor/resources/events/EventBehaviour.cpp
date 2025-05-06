//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventBehaviour.h"

#include "Event.h"


editor::resources::events::EventBehaviour::EventBehaviour(Event* event) :
    _event(event) {
}

editor::resources::events::EventBehaviour::~EventBehaviour() = default;

bool editor::resources::events::EventBehaviour::writeToEngine(std::ostream& behaviours, EventBuildDependencies& dependencies) {
    dependencies.requireDependencies.insert(getID());
    behaviours << getID() << ":new(";
    if (!writeParamsToEngine(behaviours, dependencies))
        return false;
    behaviours << "),\n";
    return true;

}
