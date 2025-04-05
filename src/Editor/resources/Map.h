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

namespace editor::resources {
    class Object;

    class Map : public EditorResource {
    public:
        Map(int mapWidth, int mapHeight, int layers);

        void writeToLua() final;

        void readFromLua() final;

        void writeToEngineLua() final;

        std::vector<std::vector<ImTextureID>>& getTiles();

        std::vector<bool>& getCollisions();

        int getMapWidth() const;

        int getMapHeight() const;

        int getLayers() const;

        void setLayers(int layers);
    private:
        int _mapWidth = 0;
        int _mapHeight = 0;
        int _layers = 0;

        std::vector<std::vector<ImTextureID>> _tiles;
        std::vector<bool> _collisions;
        std::unordered_map<int, Object*> _objects;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAP_H
