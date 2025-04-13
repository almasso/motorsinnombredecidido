//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef SPRITE_H
#define SPRITE_H

#include "EditorResource.h"
#include <filesystem>

namespace editor {
    class Project;
}

namespace editor::resources {
    class Sprite : public EditorResource {
    public:
        explicit Sprite(Project* project);
        ~Sprite();

        void init(std::string const& name, std::filesystem::path const& path, int x, int y, int w, int h);

        bool readFromLua(std::string const& name) final;

        void writeToLua() final;

        void writeToEngineLua(const std::string &platform) final;

        const std::string& getName() const;
        const std::filesystem::path& getSource() const;

        bool isInitialized() const;

        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;

    private:
        static std::filesystem::path _spritesDirectory;
        bool _init = false;
        std::string _name;
        int _x;
        int _y;
        int _width;
        int _height;
        std::filesystem::path _source;
        Project* _project;
    };

}

#endif //SPRITE_H
