//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ARECOLLIDINGCONDITION_H
#define ARECOLLIDINGCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(CollidesWithPlayerCondition, "CollidesWithPlayer") {
    public:
        CollidesWithPlayerCondition(Event* event);
        ~CollidesWithPlayerCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies, Object const* container) override;
        bool writeParams(sol::table& params) override;
    };

}

#endif //ARECOLLIDINGCONDITION_H
