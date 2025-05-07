//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <filesystem>
#include <string>
#include <render/WindowItem.h>

struct ImFont;

namespace editor {
    class Project;
}

namespace editor::render::tabs {
    class GeneralSettings : public WindowItem {
    public:
        GeneralSettings(editor::Project* project);

        void save();

        ~GeneralSettings() override;
    private:
        Project* _project = nullptr;
        bool _somethingModified = false;
        std::filesystem::path _filePath;
        std::string _gameName;
        int _cameraSize[2] = { 18, 12 };
        float _textColor[4] = {0, 0, 0, 1};
        float _backgroundColor[4] = {1, 1, 1, 1};
        std::filesystem::path _font;
        bool _fontModified = false;
        ImFont* _previewFont = nullptr;
        int _fontSize = 24;
        int _sliderValue = _fontSize;
        float _masterVolume = 1.0f;
        float _musicVolume = 0.5f;
        float _sfxVolume = 1.0f;
        void onRender() override;
        void beforeRender() override;
        void drawSettings();
    };
};

#endif //GENERALSETTINGS_H
