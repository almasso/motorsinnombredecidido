//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MODIFYLOCALVARIABLEBEHAVIOUR_H
#define MODIFYLOCALVARIABLEBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(ModifyVariableBehaviour) {
    public:
        ModifyVariableBehaviour(Event* event);
        ~ModifyVariableBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        bool _isPlayerVariable;
        char* _variable;
        char* _newValue;
        std::string _playerVariable;

        bool renderIsPlayerVariable();
        bool renderPlayerVariable();
        bool renderVariable();
        bool renderValue();
    };

}

#endif //MODIFYLOCALVARIABLEBEHAVIOUR_H
