//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef PLAYSFXBEHAVIOUR_H
#define PLAYSFXBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(PlaySFXBehaviour) {
    public:
        PlaySFXBehaviour(Event* event);
        ~PlaySFXBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        std::string _sfxHandler;
    };

}

#endif //PLAYSFXBEHAVIOUR_H
