//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ONSTARTCONDITION_H
#define ONSTARTCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(OnStartCondition, "OnStart") {
    public:
        ~OnStartCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    };

}

#endif //ONSTARTCONDITION_H
