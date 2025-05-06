//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ONSTARTCONDITION_H
#define ONSTARTCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(OnStartCondition, "OnStart") {
    public:
        OnStartCondition(Event* event);
        ~OnStartCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) override;
        bool writeParams(sol::table& params) override;
    };

}

#endif //ONSTARTCONDITION_H
