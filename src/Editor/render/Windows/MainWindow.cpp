//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MainWindow.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include "render/WindowStack.h"
#include "render/WindowItems/MapEditor.h"
#include "render/WindowItems/EventEditor.h"
#include "render/WindowItems/MapConnections.h"
#include "render/WindowItems/GeneralSettings.h"
#include <render/WindowItems/PlayerSettings.h>
#include "render/WindowItems/SpriteAnimViewer.h"
#include "utils/IconsFontAwesome6.h"

editor::render::windows::MainWindow::MainWindow(editor::Project *project) : Window("mainWindow"), _project(project) {
    editor::render::RenderManager::GetInstance().hideWindow();
    editor::render::RenderManager::GetInstance().setWidth(1920);
    editor::render::RenderManager::GetInstance().setHeight(1080);
    editor::render::RenderManager::GetInstance().enableWindowResizing();
    editor::render::RenderManager::GetInstance().centerWindowInScreen();
    editor::render::RenderManager::GetInstance().setWindowName(editor::render::RenderManager::GetInstance().getWindowName() + " - " + _project->getName());
    editor::render::WindowStack::removeAllWindowsFromStack();
    editor::render::WindowStack::addWindowToStack(this);
    editor::render::RenderManager::GetInstance().showWindow();
    _mapEditor = new tabs::MapEditor(_project);
    _eventEditor = new tabs::EventEditor(_project);
    _mapConnections = new tabs::MapConnections(_project);
    _playerSettings = new tabs::PlayerSettings(_project);
    _generalSettings = new tabs::GeneralSettings(_project);
    _spriteEditor = new tabs::SpriteAnimViewer(_project);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoScrollWithMouse |
                             ImGuiWindowFlags_NoDocking;
    _windowFlags |= flags;
    _buildTarget = _project->getPlatform();
}

void editor::render::windows::MainWindow::beforeRender() {
    ImGui::SetNextWindowSize(ImVec2(RenderManager::GetInstance().getWidth(), RenderManager::GetInstance().getHeight()), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
}

void editor::render::windows::MainWindow::onRender() {
    ImGui::BeginChild("##mainWindowToolbar");
    ImGui::SetCursorPosX(RenderManager::GetInstance().getWidth() - 350);

    ImGui::BeginDisabled(_project->getMaps().empty());
    std::string targets[] = {"Windows", "MacOS", "Linux", "Android"};
    ImGui::SetNextItemWidth(150.0f);
    if (ImGui::BeginCombo("##buildTargetDropdown", _buildTarget.c_str())) {
        for(const std::string& target : targets) {
            bool isSelected = (_buildTarget == target);
            if(ImGui::Selectable(target.c_str(), isSelected)) {
                _buildTarget = target;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
    if(ImGui::Button("##BuildButton", {60, 60})) {
        build(_buildTarget);
    }
    ImGui::EndDisabled();
    ImVec2 pos = ImGui::GetItemRectMin();
    ImGui::SetCursorPos({pos.x + 5, pos.y - 35});
    ImGui::TextColored({0, 255, 0, 255}, ICON_FA_PLAY);
    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::SetCursorPos({0, 50});
    ImGui::BeginChild("##mainWindowtabbar");
    ImGui::BeginTabBar("tabBar");
    _mapEditor->render();
    _eventEditor->render();
    _mapConnections->render();
    _playerSettings->render();
    _generalSettings->render();
    _spriteEditor->render();
    ImGui::EndTabBar();
    ImGui::EndChild();
}

void editor::render::windows::MainWindow::build(const std::string &platform) {
    _mapEditor->save();
    _eventEditor->save();
    _mapConnections->save();
    _playerSettings->save();
    _generalSettings->save();
    _spriteEditor->save();
    _mapConnections->getAdjacentMaps();
    sol::table playerTable = _playerSettings->buildPlayer();
    sol::table sceneTable = _generalSettings->buildOverworldScene(playerTable);
    std::array<float,3> audio = _generalSettings->getAudioSettings();
    _project->build(platform, sceneTable, audio, _generalSettings->usingGenericFont());
}
