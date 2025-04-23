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

        Object(Project *project, int x, int y);

        ~Object();
        bool read(sol::table const& objectTable);
        bool write(sol::table& objectTable);
        bool writeToEngine(sol::table& objectTable);

        const std::string& getSprite() const;
        void setSprite(const std::string& sprite);
        int getX() const;
        int getY() const;
        int getLayer() const;
        void setX(int x);
        void setY(int y);
        void setLayer(int layer);
        bool getCollide() const;
        void setCollide(bool collide);
        void addVariable(const std::string &key);
        void setVariable(const std::string &key, const std::string &value);
        void removeVariable(const std::string &key);
        const std::unordered_map<std::string, sol::object>& getVariables() const;
        void addEvent(events::Event* event);
        std::vector<events::Event *>::iterator removeEvent(
            std::vector<events::Event *>::iterator iter);
        std::vector<events::Event*>& getEvents();

    private:
        Project* _project;

        int _x, _y, _layer;
        bool _collides;
        std::string _spriteName;
        std::vector<events::Event*> _events;
        std::unordered_map<std::string, sol::object> _localVariables;


        bool readEvents(sol::table const& events);
        bool readLocalVars(sol::table const& localVars);
        bool writeEvents(sol::table& events);
        void writeLocalVars(sol::table& localVars);
    };
}


#endif //OBJECT_H
