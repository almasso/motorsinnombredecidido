//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef PLAYERSETTINGS_H
#define PLAYERSETTINGS_H
#include <filesystem>
#include <string>
#include <unordered_map>
#include <render/WindowItem.h>
#include <sol/object.hpp>

namespace editor {
    class Project;
}

namespace editor::render::tabs {
    class PlayerSettings : public WindowItem {
    public:
        PlayerSettings(editor::Project* project);

        void save();
        sol::table buildPlayer();
        ~PlayerSettings() override;
    private:
        editor::Project* _project = nullptr;
        bool _somethingModified = false;
        int _layer = 0, _moveSpeed = 1;
        std::filesystem::path _filePath;
        std::string _spriteName;
        std::array<std::string,4> _moveAnimation;
        std::unordered_map<std::string, sol::object> _localVariables;
        void onRender() override;
        void beforeRender() override;
        void drawSettings();
    };
};

#endif //PLAYERSETTINGS_H
