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
#include <sol/forward.hpp>

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
        void writeToEngineLua() final;

        std::vector<std::vector<Tile*>>& getTiles();

        const std::string& getName() const;

        std::vector<bool>& getCollisions();

        int getMapWidth() const;

        int getMapHeight() const;

        int getLayers() const;

        void addLayer();

        void removeLayer(int index);

        bool isInitialized() const;

        static void SetMapsDirectory(std::filesystem::path const& mapsDirectory);

    private:
        static std::filesystem::path _mapsDirectory;

        std::string _name;

        int _mapWidth;
        int _mapHeight;
        int _layers;

        bool _init = false;

        std::vector<std::vector<Tile*>> _tiles;
        std::vector<bool> _collisions;
        std::unordered_map<int, Object*> _objects;

        Project* _project;


        void writeTiles(sol::table& tiles);
        void writeCollisions(sol::table& collisions);
        bool writeObjects(sol::table& objects);

        bool readTiles(sol::table const& tiles);
        bool readCollisions(sol::table const& collisions);
        bool readObjects(sol::table const& objects);

        void handleVectorChanges();

        static std::string GetFilePath(std::string const& mapName);
    };

}


#endif //MOTORSINNOMBREDECIDIDO_MAP_H
