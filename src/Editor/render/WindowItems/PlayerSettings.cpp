//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "PlayerSettings.h"
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <render/RenderManager.h>
#include <resources/Sprite.h>

editor::render::tabs::PlayerSettings::PlayerSettings(editor::Project* project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings").c_str()), _project(project) {
    _filePath = _project->getPath() / "projectfiles"/ "settings" / "playerSettings.lua";
    sol::table playerSettings = io::LuaManager::GetInstance().getTable(_filePath.string(), true);
    if (playerSettings.valid()) {
        _layer = playerSettings["layer"].get_or(0);
        _moveSpeed = playerSettings["moveSpeed"].get_or(1);
        _spriteName = playerSettings["sprite"].get_or(std::string{});
        sol::table animationsTable = playerSettings["moveAnimations"];
        if (animationsTable.valid() && animationsTable.is<sol::table>() && animationsTable.size() == 4) {
            for (std::size_t i = 0; i < 4; ++i) {
                _moveAnimation[i] = animationsTable[i + 1].get_or(std::string{});
            }
        }
        sol::table variablesTable = playerSettings["playerVariables"];
        if (variablesTable.valid()) {
            _localVariables.clear();
            for (const auto& pair : variablesTable) {
                std::string key = pair.first.as<std::string>();
                sol::object value = pair.second;
                _localVariables[key] = value;
            }
        }
    }
}

editor::render::tabs::PlayerSettings::~PlayerSettings() {
}

void editor::render::tabs::PlayerSettings::onRender() {
    ImGui::Dummy(ImVec2(RenderManager::GetInstance().getWidth()/4, 0));
    ImGui::SameLine();
    drawSettings();
    if(_somethingModified && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        save();
    }
}

void editor::render::tabs::PlayerSettings::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::PlayerSettings::save() {
    _somethingModified = false;
    sol::table playerSettings = io::LuaManager::GetInstance().getState().create_table();
    sol::table playerVariables = io::LuaManager::GetInstance().getState().create_table();

    playerSettings["layer"] = _layer;
    playerSettings["moveSpeed"] = _moveSpeed;
    playerSettings["sprite"] = _spriteName;
    playerSettings["moveAnimations"] = sol::as_table<std::array<std::string,4>>(std::move(_moveAnimation));
    for (auto& [key, variable] : _localVariables) {
        playerVariables[key] = variable;
    }
    playerSettings["playerVariables"] = playerVariables;

    io::LuaManager::GetInstance().writeToFile(playerSettings, _filePath.string());
}

void editor::render::tabs::PlayerSettings::drawSettings() {
    ImGui::BeginChild("##settings", ImVec2(RenderManager::GetInstance().getWidth()/2, 0), true);
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings.player").c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    if (ImGui::BeginCombo("##spriteDropdown", _spriteName != "" ? _spriteName.c_str() :
        io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.spriteSelector").c_str())) {
        for(auto it = _project->getSprites().begin(); it != _project->getSprites().end(); ++it) {
            auto sprite = *it;
            bool isSelected = (_spriteName == sprite.first);
            if(ImGui::Selectable(sprite.first.c_str(), isSelected)) {
                _spriteName = sprite.first;
                _somethingModified = true;
            }
        }
        if(ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.spriteSelectorNone").c_str(),_spriteName == "")) {
            _spriteName = "";
            _somethingModified = true;
        }
        ImGui::EndCombo();
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImVec2 spriteSize(256, 256);
    float windowWidth = ImGui::GetContentRegionAvail().x;
    float offsetX = (windowWidth - spriteSize.x) * 0.5f;
    if (offsetX < 0) offsetX = 0;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
    if (!_spriteName.empty() && _project->getSprite(_spriteName)) {
        resources::Sprite* sprite = _project->getSprite(_spriteName);
        ImTextureID texId = sprite->getTextureID();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + spriteSize.x, p.y + spriteSize.y), IM_COL32(150, 150, 150, 180));
        ImGui::Image(texId, spriteSize, sprite->getSpriteCoordsMin(), sprite->getSpriteCoordsMax());
    } else {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + spriteSize.x, p.y + spriteSize.y), IM_COL32(150, 150, 150, 180));
        ImGui::Dummy(spriteSize);
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/6);
    if (ImGui::InputInt(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.spriteLayer")).c_str(), &_layer)) {
        _somethingModified = true;
    };
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/6);
    if (ImGui::InputInt(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings.playerSpeed")).c_str(), &_moveSpeed)) {
        _somethingModified = true;
    };
    ImGui::Spacing();
    ImGui::Spacing();
    for (int i = 0; i < _moveAnimation.size(); i++) {
        ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/6);
        if (ImGui::BeginCombo(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings.moveAnim" + std::to_string(i))).c_str(),
            _moveAnimation[i] != "" ? _moveAnimation[i].c_str() :
            io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings.animSelector").c_str())) {
            for(auto it = _project->getAnimations().begin(); it != _project->getAnimations().end(); ++it) {
                std::string animation = it->first;
                bool isSelected = (_moveAnimation[i] == animation);
                if(ImGui::Selectable(animation.c_str(), isSelected)) {
                    _moveAnimation[i] = animation;
                    _somethingModified = true;
                }
            }
            if(ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings.animSelectorNone").c_str(),_moveAnimation[i] == "")) {
                _moveAnimation[i] = "";
                _somethingModified = true;
            }
            ImGui::EndCombo();
        }
        ImGui::Spacing();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.playerSettings.playerVariables").c_str());
    for (auto [name, value] : _localVariables) {
        char variableValue[256];
        std::strncpy(variableValue, value.as<std::string>().c_str(), sizeof(variableValue) - 1);
        if (ImGui::InputText(name.c_str(),
            variableValue, IM_ARRAYSIZE(variableValue))) {
            auto& lua = io::LuaManager::GetInstance().getState();
            _localVariables[name] = make_object(lua,variableValue);
            _somethingModified = true;
        }
    }
    char newVariableName[256] = {};
    if (ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.addVariable").c_str(),
        newVariableName, IM_ARRAYSIZE(newVariableName), ImGuiInputTextFlags_EnterReturnsTrue)) {
        auto& lua = io::LuaManager::GetInstance().getState();
        _localVariables.insert({newVariableName,make_object(lua,"")});
        _somethingModified = true;
    }
    ImGui::EndChild();
}
