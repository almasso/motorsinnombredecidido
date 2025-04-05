//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MAP_H
#define MOTORSINNOMBREDECIDIDO_MAP_H

#include "EditorResource.h"
#include <vector>
#include <imgui.h>
#include <unordered_map>
#include <sol/forward.hpp>

namespace editor::resources {
    class Object;
    struct Tile;

    class Map : public EditorResource {
    public:
        Map(int mapWidth, int mapHeight, int layers);

        void writeToLua() final;

        void readFromLua() final;

        void writeToEngineLua() final;

        std::vector<std::vector<Tile*>>& getTiles();

        std::vector<bool>& getCollisions();

        int getMapWidth() const;

        int getMapHeight() const;

        int getLayers() const;

        void setLayers(int layers);
    private:
        void writeTiles(sol::table& tiles);
        void writeCollisions(sol::table& collisions);
        bool writeObjects(sol::table& objects);

        int _mapWidth = 0;
        int _mapHeight = 0;
        int _layers = 0;

        std::vector<std::vector<Tile*>> _tiles;
        std::vector<bool> _collisions;
        std::unordered_map<int, Object*> _objects;

        std::string filename;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAP_H
