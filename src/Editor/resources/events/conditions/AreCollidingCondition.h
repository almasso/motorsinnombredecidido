//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef ARECOLLIDINGCONDITION_H
#define ARECOLLIDINGCONDITION_H

#include "../EventCondition.h"

namespace editor::resources::events {

    class EventConditionClass(AreCollidingCondition, "AreColliding") {
    public:
        AreCollidingCondition(Event* event);
        ~AreCollidingCondition() override;
        bool read(sol::table const& params) override;
        bool render() override;
    protected:
        bool writeParamsToEngine(sol::table& params) override;
        bool writeParams(sol::table& params) override;
    private:
        std::string _objectA;
        std::string _objectB;
    };

}

#endif //ARECOLLIDINGCONDITION_H
