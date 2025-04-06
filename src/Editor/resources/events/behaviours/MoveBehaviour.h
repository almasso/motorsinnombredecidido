//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOVEBEHAVIOUR_H
#define MOVEBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(MoveBehaviour) {
        ~MoveBehaviour() override;
        bool read(sol::table const& condition) override;
        bool write(sol::table& condition) override;
        bool writeToEngine(sol::table& condition) override;
    };

}

#endif //MOVEBEHAVIOUR_H
