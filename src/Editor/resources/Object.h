//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <unordered_map>
#include <sol/lua_value.hpp>

#include "Sprite.h"


namespace editor::resources {
    namespace events {
        class Event;
    }

    class Object {
    public:
        explicit Object(Project* project);
        ~Object();
        bool read(sol::table const& objectTable);
        bool write(sol::table& objectTable);
        bool writeToEngine(sol::table& objectTable);

        int getX() const;
        int getY() const;
    private:
        Project* _project;

        int _x, _y;
        bool _collidable;
        std::vector<events::Event*> _events;
        std::unordered_map<std::string, sol::lua_value> _localVariables;


        bool readEvents(sol::table const& events);
        bool readLocalVars(sol::table const& localVars);
        bool writeEvents(sol::table& events);
        void writeLocalVars(sol::table& localVars);
    };
}


#endif //OBJECT_H
