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
        JumpBehaviour();
        ~JumpBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        int _target;
    };

}

#endif //JUMPBEHAVIOUR_H
