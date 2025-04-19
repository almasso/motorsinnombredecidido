//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MAP_H
#define MOTORSINNOMBREDECIDIDO_MAP_H

#include "EditorResource.h"
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <render/WindowItems/MapConnections.h>
#include <sol/forward.hpp>
#include <Utils/Vector2.h>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Object;
    struct Tile;

    class Map : public EditorResource {
    public:

        Map(Project* project);
        ~Map();

        void init(std::string const& name, int mapWidth, int mapHeight, int layers);
        bool readFromLua(std::string const& name) final;

        void writeToLua() final;
        void writeToEngineLua(const std::string &platform) final;

        std::vector<std::vector<Tile*>>& getTiles();

        const std::string& getName() const;

        std::vector<bool>& getCollisions();

        int getMapWidth() const;

        int getMapHeight() const;

        int getMapX() const;

        int getMapY() const;

        void setMapX(int x);

        void setMapY(int y);

        int getLayers() const;

        void addLayer();

        void removeLayer(int index);

        int getWorld() const;

        void setWorld(int i);

        bool isInitialized() const;


        static void SetMapsDirectory(std::filesystem::path const& mapsDirectory);

        bool isAdjacent(const Map *other) const;

        bool isOverlapping(const Map *other) const;

        void addAdjacent(const std::string &adjacent);

        void clearAdjacent();

    private:
        static std::filesystem::path _mapsDirectory;

        std::string _name;

        int _mapWidth;
        int _mapHeight;
        int _mapX;
        int _mapY;
        int _layers;
        int _world;

        bool _init = false;

        std::vector<std::vector<Tile*>> _tiles;
        std::vector<bool> _collisions;
        std::vector<std::string> _adjacent;
        std::unordered_map<int, Object*> _objects;

        Project* _project;

        void writeTiles(sol::table& tiles);
        void writeCollisions(sol::table& collisions);
        bool writeObjects(sol::table& objects);

        void writeComponents(sol::table &components);
        void writeChildren(sol::table &children);

        bool readTiles(sol::table const& tiles);
        bool readCollisions(sol::table const& collisions);
        bool readObjects(sol::table const& objects);

        void handleVectorChanges();

        static std::string GetFilePath(std::string const& mapName);
    };

}


#endif //MOTORSINNOMBREDECIDIDO_MAP_H
