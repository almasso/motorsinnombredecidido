//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef WAITFORBEHAVIOUR_H
#define WAITFORBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {
    class EventCondition;

    class EventBehaviourClass(WaitForBehaviour) {
    public:
        WaitForBehaviour(Event* event);
        ~WaitForBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        bool renderConditionSelector(EventCondition*& condition);

    private:
        EventCondition* _condition;
    };

}

#endif //WAITFORBEHAVIOUR_H
