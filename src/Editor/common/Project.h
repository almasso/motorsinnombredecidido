//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_PROJECT_H
#define RPGBAKER_PROJECT_H

#include <string>
#include <filesystem>
#include <unordered_set>
#include <ctime>
#include <resources/events/conditions/AndCondition.h>
#include <sol/sol.hpp>

namespace editor::resources {
    class Tileset;
    class Map;
    class Sprite;
    class Animation;
    namespace events {
        class Event;
    }
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

        bool build(const std::string &platform, const sol::table &overWorldScene, const std::array<float, 3> &audio, bool genericFont);

        std::unordered_map<std::string, std::vector<std::string>> getAdjacentMaps();

        void buildSettings(const std::string &platform);

        void buildAudioSettings(const std::string &platform, const std::array<float, 3> &audio);

        const std::string& getName() const;

        int getMaxLayers() const;

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

        void saveEverything();

        void addMap(editor::resources::Map* map);

        void refreshMaps();

        int totalMaps() const;

        const std::unordered_map<std::string, editor::resources::Map*>& getMaps() const;

        template <std::convertible_to<std::string> T>
        editor::resources::Map* getMap(T&& name) {
            auto m = _maps.find(std::forward<T>(name));
            if(m == _maps.end()) return nullptr;
            else return m->second;
        }

        template <std::convertible_to<std::string> T>
        std::unordered_map<std::string, editor::resources::Map*>::iterator removeMap(T&& name) {
            auto m = _maps.find(std::forward<T>(name));
            if(m == _maps.end()) return m;
            return _maps.erase(m);
        }

        void addTileset(editor::resources::Tileset* tileset);

        int totalTilesets() const;

        const std::unordered_map<std::string, editor::resources::Tileset*>& getTilesets() const;

        void refreshTilesets();

        template <std::convertible_to<std::string> T>
        editor::resources::Tileset* getTileset(T&& name) {
            auto t = _tilesets.find(std::forward<T>(name));
            if(t == _tilesets.end()) return nullptr;
            return t->second;
        }

        template <std::convertible_to<std::string> T>
        std::unordered_map<std::string, editor::resources::Tileset*>::iterator removeTileset(T&& name) {
            auto t = _tilesets.find(std::forward<T>(name));
            if(t == _tilesets.end()) return t;
            return _tilesets.erase(t);
        }

        void addEvent(editor::resources::events::Event* event);

        int totalEvents() const;

        const std::unordered_map<std::string, editor::resources::events::Event*>& getEvents() const;

        void refreshEvents();

        template <std::convertible_to<std::string> T>
        editor::resources::events::Event* getEvent(T&& name) {
            auto t = _events.find(std::forward<T>(name));
            if(t == _events.end()) return nullptr;
            return t->second;
        }

        template <std::convertible_to<std::string> T>
        std::unordered_map<std::string, editor::resources::events::Event*>::iterator removeEvent(T&& name) {
            auto t = _events.find(std::forward<T>(name));
            if(t == _events.end())
                return t;
            t->second->eraseFromLua();
            return _events.erase(t);
        }

        void addSprite(editor::resources::Sprite* sprite);

        int totalSprites() const;

        const std::unordered_map<std::string, editor::resources::Sprite*>& getSprites() const;

        void refreshSprites();

        template <std::convertible_to<std::string> T>
        editor::resources::Sprite* getSprite(T&& name) {
            auto t = _sprites.find(std::forward<T>(name));
            if(t == _sprites.end()) return nullptr;
            return t->second;
        }

        template <std::convertible_to<std::string> T>
        std::unordered_map<std::string, editor::resources::Sprite*>::iterator removeSprite(T&& name) {
            auto t = _sprites.find(std::forward<T>(name));
            if(t == _sprites.end()) return t;
            return _sprites.erase(t);
        }

        void addAnimation(editor::resources::Animation* animation);

        int totalAnimations() const;

        const std::unordered_map<std::string, editor::resources::Animation*>& getAnimations() const;

        void refreshAnimations();

        template <std::convertible_to<std::string> T>
        editor::resources::Animation* getAnimation(T&& name) {
            auto t = _animations.find(std::forward<T>(name));
            if(t == _animations.end()) return nullptr;
            return t->second;
        }

        template <std::convertible_to<std::string> T>
        std::unordered_map<std::string, editor::resources::Animation*>::iterator removeAnimation(T&& name) {
            auto t = _animations.find(std::forward<T>(name));
            if(t == _animations.end()) return t;
            return _animations.erase(t);
        }

        bool isSpriteBeingUsedInAnim(editor::resources::Sprite* sprite, std::vector<std::string>& names);

        std::filesystem::path getAssetsPath() const;

        std::filesystem::path getBuildPath(const std::string &platform) const;

        void setPlayerLocalVariables(std::unordered_map<std::string, sol::object>* localVariables);
        std::unordered_map<std::string, sol::object>* getPlayerLocalVariables() const;

    private:
        std::string _name;
        std::filesystem::path _projectPath;
        std::unordered_set<std::filesystem::path> _extraRoutes;
        std::tm _lastModified;
        bool _setToDelete = false;
        bool _found = false;

        std::unordered_map<std::string, editor::resources::Tileset*> _tilesets;
        std::unordered_map<std::string, editor::resources::Sprite*> _sprites;
        std::unordered_map<std::string, editor::resources::Animation*> _animations;
        std::unordered_map<std::string, editor::resources::Map*> _maps;
        std::unordered_map<std::string, editor::resources::events::Event*> _events;

        std::unordered_map<std::string, sol::object>* _localVariables;

        int _dimensions[2];

        void findProject();

        void loadProject(const sol::table& project);

        void saveProject();

        void buildSprites(std::string const& platform);
        void buildAnimations(std::string const& platform);
        void launchBuild();
    };
}


#endif //RPGBAKER_PROJECT_H
