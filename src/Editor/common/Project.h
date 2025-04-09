//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_PROJECT_H
#define MOTORSINNOMBREDECIDIDO_PROJECT_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <ctime>
#include <sol/sol.hpp>

namespace editor::resources {
    class Tileset;
    class Map;
}

namespace editor {
    class Project {
    public:
        template<std::convertible_to<std::string> T>
        Project(T&& name, T&& path, int* dimensions) : _name(std::forward<T>(name)), _projectPath(std::forward<T>(path)) {
            std::copy(dimensions, dimensions + 2, std::begin(_dimensions));
            _found = true;
            updateLastModified();
            saveProject();
        }

        template<std::convertible_to<std::string> T>
        Project(T&& path) {
            updatePath(path);
        }

        void initResources();

        bool build(const std::string &platform);

        std::unordered_map<std::string, std::vector<std::string>> getAdjacentMaps();

        void buildSettings(const std::string &platform);

        void buildAudioSettings(const std::string &platform);

        void buildOverworldScene(const std::string &platform);

        const std::string& getName() const;

        template <std::convertible_to<std::string> T>
        void setName(T&& name) {
            _name = std::forward<T>(name);
        }

        template <std::convertible_to<std::string> T>
        void updateName(T&& name) {
            setName(name);
            updateLastModified();
            saveProject();
        }

        const std::filesystem::path& getPath() const;

        template <std::convertible_to<std::string> T>
        void updatePath(T&& path) {
            _projectPath = std::forward<T>(path);
            findProject();
        }

        template <std::convertible_to<std::string> T>
        void addExtraRoute(T&& route) {
            _extraRoutes.insert(std::forward<T>(route));
            saveProject();
        }

        const std::unordered_set<std::filesystem::path>& getExtraRoutes() const;

        const std::tm& getLastModificationTime() const;

        const int* getDimensions() const;

        bool isSetToDelete() const;

        void setToDelete();

        bool isFound() const;

        void updateLastModified();

        void addMap(editor::resources::Map* map);

        int totalMaps() const;

        void saveEverything();

        const std::unordered_map<std::string, editor::resources::Map*>& getMaps() const;

        template <std::convertible_to<std::string> T>
        editor::resources::Map* getMap(T&& name) {
            auto m = _maps.find(std::forward<T>(name));
            if(m == _maps.end()) return nullptr;
            else return m->second;
        }

        template <std::convertible_to<std::string> T>
        void removeMap(T&& name) {
            auto m = _maps.find(std::forward<T>(name));
            if(m == _maps.end()) return;
            else _maps.erase(m);
        }

        void addTileset(editor::resources::Tileset* tileset);

        int totalTilesets() const;

        const std::unordered_map<std::string, editor::resources::Tileset*>& getTilesets() const;

        template <std::convertible_to<std::string> T>
        editor::resources::Tileset* getTileset(T&& name) {
            auto t = _tilesets.find(std::forward<T>(name));
            if(t == _tilesets.end()) return nullptr;
            else return t->second;
        }

        template <std::convertible_to<std::string> T>
        void removeTileset(T&& name) {
            auto t = _tilesets.find(std::forward<T>(name));
            if(t == _tilesets.end()) return;
            else _tilesets.erase(t);
        }

        std::filesystem::path getAssetsPath() const;

        std::filesystem::path getBuildPath(const std::string &platform) const;

    private:
        std::string _name;
        std::filesystem::path _projectPath;
        std::unordered_set<std::filesystem::path> _extraRoutes;
        std::tm _lastModified;
        bool _setToDelete = false;
        bool _found = false;

        std::unordered_map<std::string, editor::resources::Tileset*> _tilesets;
        std::unordered_map<std::string, editor::resources::Map*> _maps;

        int _dimensions[2];

        void findProject();

        void loadProject(const sol::table& project);

        void saveProject();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_PROJECT_H
