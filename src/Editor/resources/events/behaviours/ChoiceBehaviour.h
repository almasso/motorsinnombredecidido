//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef CHOICEBEHAVIOUR_H
#define CHOICEBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(ChoiceBehaviour) {
    public:
        ChoiceBehaviour();
        ~ChoiceBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        std::string _variable;
        std::vector<std::pair<std::string, sol::lua_value>> _options;
    };

}

#endif //CHOICEBEHAVIOUR_H
