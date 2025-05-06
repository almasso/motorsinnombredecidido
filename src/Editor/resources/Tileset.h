//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_TILESET_H
#define RPGBAKER_TILESET_H

#include "EditorResource.h"
#include <vector>
#include <filesystem>
#include <sol/sol.hpp>

namespace editor
{
    class Project;
}

namespace editor::resources {
    struct Tile;

    class Tileset : public EditorResource {
    public:
        explicit Tileset(Project* project);
        ~Tileset();

        void init(std::string const& name, std::filesystem::path const& source, int offsetX = 0, int offsetY = 0);
        bool readFromLua(std::string const& name) final;

        void writeToLua() final;
        void writeToEngineLua(const std::string &platform) final;

        int getXTiles() const;
        int getYTiles() const;

        const std::vector<Tile*>& getTiles() const;
        const std::filesystem::path& getSource() const;
        const std::string& getName() const;
        std::vector<bool>& getCollisions();

        bool isInitialized() const;

        int getOffsetX() const;
        int getOffsetY() const;

        static void SetTilesetsDirectory(std::filesystem::path const& tilesetsDirectory);

    private:
        static std::filesystem::path _tilesetsDirectory;

        bool _init = false;
        std::string _name;
        std::filesystem::path _source;
        std::vector<Tile*> _tiles;
        std::vector<bool> _collisions;
        int _offsetX;
        int _offsetY;
        int _xTiles;
        int _yTiles;
        Project* _project;

        void generateTileset();

        static std::string GetFilePath(std::string const& mapName);

        void init(const std::string& name, const std::filesystem::path& source, const sol::table& collisions, int offsetX = 0, int offsetY = 0);
    };
}


#endif //MOTORSINNOMBREDECIDIDO_TILESET_H
