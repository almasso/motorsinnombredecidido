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
        AlwaysCondition(Event* event);
        ~AlwaysCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    };

}

#endif //ALWAYSCONDITION_H
