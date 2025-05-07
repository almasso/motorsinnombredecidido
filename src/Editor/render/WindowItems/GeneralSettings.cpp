//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "GeneralSettings.h"

#include <common/EditorError.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <render/RenderManager.h>
#include <resources/Sprite.h>
#include <utils/tinyfiledialogs/tinyfiledialogs.h>


editor::render::tabs::GeneralSettings::GeneralSettings(editor::Project* project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings").c_str()), _project(project) {
    _filePath = _project->getPath() / "projectfiles" / "settings" / "generalSettings.lua";
    sol::table settings = io::LuaManager::GetInstance().getTable(_filePath.string(), true);
    if (settings.valid()) {
        _gameName = settings["gameName"].get_or(_project->getName());
        sol::object cameraSize = settings["cameraSize"];
        if (cameraSize.valid() && cameraSize.is<sol::table>()){
            sol::table cameraSizeTable = cameraSize;
            if (cameraSizeTable.size() == 2) {
                for (std::size_t i = 0; i < 2; ++i) {
                    _cameraSize[i] = cameraSizeTable[i + 1].get_or(18 - 6*i);
                }
            }
        }
        sol::object textColor = settings["textColor"];
        if (textColor.valid() && textColor.is<sol::table>()) {
            sol::table textColorTable = textColor;
            if (textColorTable.size() == 4) {
                for (std::size_t i = 0; i < 4; ++i) {
                    _textColor[i] = textColorTable[i + 1].get_or(i == 3 ? 1.0f : 0.0f);
                }
            }
        }
        sol::object backgroundColor = settings["backgroundColor"];
        if (backgroundColor.valid() && backgroundColor.is<sol::table>()) {
            sol::table backgroundColorTable = backgroundColor;
            if (backgroundColorTable.size() == 4) {
                for (std::size_t i = 0; i < 4; ++i) {
                    _backgroundColor[i] = backgroundColorTable[i + 1].get_or(1.0f);
                }
            }
        }
        _font = std::filesystem::path(settings["font"].get_or(std::string()));
        _fontSize = settings["fontSize"].get_or(24);
        _sliderValue = _fontSize;
        _fontModified = true;
        _masterVolume = settings["masterVolume"].get_or(1.0f);
        _musicVolume = settings["musicVolume"].get_or(0.5f);
        _sfxVolume = settings["sfxVolume"].get_or(1.0f);
    }
}

editor::render::tabs::GeneralSettings::~GeneralSettings() {
}

void editor::render::tabs::GeneralSettings::onRender() {
    ImGui::Dummy(ImVec2(RenderManager::GetInstance().getWidth()/4, 0));
    ImGui::SameLine();
    drawSettings();
    if(_somethingModified && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        save();
    }
}

void editor::render::tabs::GeneralSettings::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::GeneralSettings::save() {
    _somethingModified = false;
    sol::table settings = io::LuaManager::GetInstance().getState().create_table();
    settings["gameName"] = _gameName;
    settings["cameraSize"] = sol::as_table<std::array<int,2>>({_cameraSize[0], _cameraSize[1]});
    settings["textColor"] = sol::as_table<std::array<float,4>>({_textColor[0], _textColor[1],_textColor[2], _textColor[3]});
    settings["backgroundColor"] = sol::as_table<std::array<float,4>>({_backgroundColor[0], _backgroundColor[1],_backgroundColor[2], _backgroundColor[3]});
    settings["font"] = _font.string();
    settings["fontSize"] = _fontSize;
    settings["masterVolume"] = _masterVolume;
    settings["musicVolume"] = _musicVolume;
    settings["sfxVolume"] = _sfxVolume;
    io::LuaManager::GetInstance().writeToFile(settings, _filePath.string());
}

void editor::render::tabs::GeneralSettings::drawSettings() {
    ImGui::BeginChild("##settings", ImVec2(RenderManager::GetInstance().getWidth()/2, 0), true);
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.settings").c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.gameName").c_str());
    char gameName[256];
    std::strncpy(gameName, _gameName.c_str(), sizeof(gameName) - 1);
    if (ImGui::InputText("##settings", gameName, IM_ARRAYSIZE(gameName))) {
        _gameName = gameName;
        _somethingModified = true;
    };
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.camera").c_str());
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/4);
    if (ImGui::InputInt2(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.cameraSize").c_str(), _cameraSize)) {
        _somethingModified = true;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.text").c_str());
    ImGui::Spacing();
    if (!_font.empty()) {
        ImGui::Text("%s", _font.filename().stem().string().c_str());
        ImGui::SameLine();
    }
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.selectFont").c_str())) {
        const char* fileExtension[] = {"*.ttf"};
        const char* route = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.selectFont").c_str(),
                (_project->getPath() / "assets/").string().c_str(),
                1,
                fileExtension,
                nullptr,
                0
        );
        if(route != nullptr) {
            if(std::filesystem::path(route).parent_path() != (_project->getPath() / "assets")) {
                showUserWarning(io::LocalizationManager::GetInstance().getString("error.assetlocationnotvalid"))
            }
            else {
                std::filesystem::path selectedPath(route);
                _font = selectedPath;
                _fontModified = true;
                _somethingModified = true;
            }
        }
    }
    ImGui::Spacing();
    if(ImGui::ColorEdit4(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.textColor").c_str(), _textColor)) {
        _somethingModified = true;
    }
    ImGui::Spacing();
    if (ImGui::ColorEdit4(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.backgroundColor").c_str(), _backgroundColor)) {
        _somethingModified = true;
    }
    ImGui::Spacing();
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/6);
    ImGui::SliderInt(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.fontSize")).c_str(), &_sliderValue, 18, 72);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        _fontModified = true;
        _somethingModified = true;
        _fontSize = _sliderValue;
    }
    if (_fontModified) {
        RenderManager::GetInstance().requestFont(_font, _fontSize, _previewFont);
        _fontModified = false;
    }
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(
        _backgroundColor[0],
        _backgroundColor[1],
        _backgroundColor[2],
        _backgroundColor[3]
    ));
    ImVec2 childSize(ImGui::GetContentRegionAvail().x, 150.0f);
    ImGui::BeginChild("FontPreviewChild", childSize, true);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(
        _textColor[0],
        _textColor[1],
        _textColor[2],
        _textColor[3]
    ));
    if (_previewFont)
        ImGui::PushFont(_previewFont);
    float textHeight = static_cast<float>(_fontSize);
    ImGui::SetCursorPosY((childSize.y - textHeight) * 0.5f);
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.textPreview").c_str());
    if (_previewFont)
        ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.audio").c_str());
    ImGui::SliderFloat(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.masterVolume")).c_str(),
        &_masterVolume, 0, 1);
    ImGui::Spacing();
    ImGui::SliderFloat(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.musicVolume")).c_str(),
        &_musicVolume, 0, 1);
    ImGui::Spacing();
    ImGui::SliderFloat(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.sfxVolume")).c_str(),
        &_sfxVolume, 0, 1);
    ImGui::Spacing();
    ImGui::EndChild();
}
