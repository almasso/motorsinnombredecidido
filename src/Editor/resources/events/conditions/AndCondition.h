//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ANDCONDITION_H
#define ANDCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(AndCondition, "And") {
    public:
        AndCondition();
        ~AndCondition() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& condition) override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        EventCondition* _conditionA;
        EventCondition* _conditionB;
    };

}

#endif //ANDCONDITION_H
