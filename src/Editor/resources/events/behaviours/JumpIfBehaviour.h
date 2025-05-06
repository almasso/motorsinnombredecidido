//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef JUMPIFBEHAVIOUR_H
#define JUMPIFBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {
    class EventCondition;

    class EventBehaviourClass(JumpIfBehaviour) {
    public:
        JumpIfBehaviour(Event* event);
        ~JumpIfBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        int _target;
        EventCondition* _condition;

        bool renderConditionSelector(EventCondition*& condition);
        bool renderBehaviourSelector();
    };

}

#endif //JUMPIFBEHAVIOUR_H
