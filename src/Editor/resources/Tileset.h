//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_TILESET_H
#define MOTORSINNOMBREDECIDIDO_TILESET_H

#include "EditorResource.h"
#include <vector>
#include <filesystem>

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
        int _offsetX;
        int _offsetY;
        int _xTiles;
        int _yTiles;
        Project* _project;

        void generateTileset();

        static std::string GetFilePath(std::string const& mapName);
    };
}


#endif //MOTORSINNOMBREDECIDIDO_TILESET_H
