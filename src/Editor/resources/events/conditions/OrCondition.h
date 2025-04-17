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
        OrCondition();
        ~OrCondition() override;
        bool read(sol::table const& params) override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    private:
        EventCondition* _conditionA;
        EventCondition* _conditionB;
    };

}

#endif //ORCONDITION_H
