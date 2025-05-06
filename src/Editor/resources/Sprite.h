//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef SPRITE_H
#define SPRITE_H

#include "EditorResource.h"
#include "imgui.h"
#include <filesystem>

namespace editor {
    class Project;
}

typedef unsigned long long ImU64;
typedef ImU64 ImTextureID;

namespace editor::resources {
    class Sprite : public EditorResource {
    public:
        explicit Sprite(Project* project);
        ~Sprite();

        void init(std::string const& name, std::filesystem::path const& path, ImTextureID textureID, int x, int y, int w, int h);

        bool readFromLua(std::string const& name) final;

        void writeToLua() final;

        void writeToEngineLua(const std::string &platform) final;

        const std::string& getName() const;
        const std::filesystem::path& getSource() const;
        const ImTextureID getTextureID() const;

        bool isInitialized() const;

        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;

        ImVec2 getSpriteCoordsMin() const;

        ImVec2 getSpriteCoordsMax() const;

        static void setSpritesDirectory(const std::filesystem::path& spritesDirectory);

    private:
        static std::filesystem::path _spritesDirectory;
        bool _init = false;
        std::string _name;
        int _x;
        int _y;
        int _width;
        int _height;
        std::filesystem::path _source;
        ImTextureID _textureID;
        Project* _project;

        static std::string getFilePath(const std::string& spriteName);

        void init(const std::string& name, const std::filesystem::path& path, int x, int y, int w, int h);
    };

}

#endif //SPRITE_H
