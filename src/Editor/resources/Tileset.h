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
    class Tileset : public EditorResource {
    public:
        template <std::convertible_to<std::filesystem::path> T>
        Tileset(T&& filepath) : EditorResource("tileset"), _source(std::forward<T>(filepath)) {}

        void writeToLua() final;

        void readFromLua() final;

        void writeToEngineLua() final;

        const std::vector<ImTextureID>& getTiles() const;

        const std::filesystem::path& getSource() const;
    private:
        std::filesystem::path _source;
        std::vector<ImTextureID> _tiles;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_TILESET_H
