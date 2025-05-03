//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef NOTCONDITION_H
#define NOTCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(NotCondition, "Not") {
    public:
        NotCondition(Event* event);
        ~NotCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    private:
        EventCondition* _condition;

        bool renderConditionSelector(EventCondition*& condition);
    };

}

#endif //NOTCONDITION_H
