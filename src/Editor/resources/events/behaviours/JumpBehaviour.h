//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef JUMPBEHAVIOUR_H
#define JUMPBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(JumpBehaviour) {
    public:
        JumpBehaviour(Event* event);
        ~JumpBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        int _target;

        bool renderBehaviourSelector();
    };

}

#endif //JUMPBEHAVIOUR_H
