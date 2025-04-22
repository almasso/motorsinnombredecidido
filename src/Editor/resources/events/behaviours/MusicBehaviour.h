//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MUSICBEHAVIOUR_H
#define MUSICBEHAVIOUR_H

#include "../EventBehaviour.h"

namespace editor::resources::events {

    class EventBehaviourClass(MusicBehaviour) {
    public:
        MusicBehaviour(Event* event);
        ~MusicBehaviour() override;
        bool read(sol::table const& params) override;
        bool writeToEngine(sol::table& behaviour, std::vector<std::string>& componentDependencies) override;
        bool render() override;
    protected:
        bool writeParams(sol::table& params) override;
    private:
        enum MUSIC_ACTION : int {
            PLAY,
            STOP,
            RESUME,
            PAUSE,
            CHANGE,
            VOLUME,
            LOOP,
            MAX_ACTION
        } _action;
        union {
            char* clip;
            float volume;
            bool loop;
        } _param;

        bool renderActionSelector();
        bool renderChangeAction();
        bool renderVolumeAction();
        bool renderLoopAction();
        std::string getActionName(MUSIC_ACTION action) const;
    };

}

#endif //MUSICBEHAVIOUR_H
