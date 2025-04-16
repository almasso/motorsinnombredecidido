//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MODIFYLOCALVARIABLEBEHAVIOUR_H
#define MODIFYLOCALVARIABLEBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(ModifyLocalVariableBehaviour) {
    public:
        ModifyLocalVariableBehaviour();
        ~ModifyLocalVariableBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        std::string _variable;
        sol::lua_value _newValue;
    };

}

#endif //MODIFYLOCALVARIABLEBEHAVIOUR_H
