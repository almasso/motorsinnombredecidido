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
        bool writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies, Object const* container) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        char* _sfxSource;

        bool renderAudioSelector();
        void writeDependencies(EventBuildDependencies& dependencies, std::string const& handler);
    };

}

#endif //PLAYSFXBEHAVIOUR_H
