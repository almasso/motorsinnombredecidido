//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef INTERACTIONCONDITION_H
#define INTERACTIONCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(InteractionCondition, "Interaction") {
    public:
        InteractionCondition(Event* event);
        ~InteractionCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies) override;
        bool writeParams(sol::table& params) override;
    };
}



#endif //INTERACTIONCONDITION_H
