//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ORCONDITION_H
#define ORCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(OrCondition, "Or") {
    public:
        OrCondition(Event* event);
        ~OrCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(std::ostream& condition, EventBuildDependencies& dependencies) override;
        bool writeParams(sol::table& params) override;
    private:
        EventCondition* _conditionA;
        EventCondition* _conditionB;

        bool renderConditionSelector(EventCondition*& condition);
    };

}

#endif //ORCONDITION_H
