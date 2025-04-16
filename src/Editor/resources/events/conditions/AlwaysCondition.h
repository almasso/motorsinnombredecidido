//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ALWAYSCONDITION_H
#define ALWAYSCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(AlwaysCondition, "Always") {
    public:
        AlwaysCondition();
        ~AlwaysCondition() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& condition) override;
    protected:
        bool writeParams(sol::table& params) override;
    };

}

#endif //ALWAYSCONDITION_H
