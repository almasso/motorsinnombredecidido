//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ONSTARTCONDITION_H
#define ONSTARTCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(OnStartCondition) {
        ~OnStartCondition() override;
        bool read(sol::table const& condition) override;
        bool write(sol::table& condition) override;
        bool writeToEngine(sol::table& condition) override;
    };

}

#endif //ONSTARTCONDITION_H
