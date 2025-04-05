//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_TILESET_H
#define MOTORSINNOMBREDECIDIDO_TILESET_H

#include "EditorResource.h"
#include <vector>
#include <imgui.h>
#include <filesystem>

namespace editor::resources {
    struct Tile;

    class Tileset : public EditorResource {
    public:
        template <std::convertible_to<std::filesystem::path> T, std::convertible_to<std::string> S>
        Tileset(T&& filepath, S&& name) : EditorResource("tileset"), _source(std::forward<T>(filepath)),
        _name(std::forward<S>(name)) {}

        void writeToLua() final;

        void readFromLua() final;

        void writeToEngineLua() final;

        const std::vector<Tile*>& getTiles() const;

        const std::filesystem::path& getSource() const;

        const std::string& getName() const;
    private:
        std::string _name;
        std::filesystem::path _source;
        std::vector<Tile*> _tiles;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_TILESET_H
