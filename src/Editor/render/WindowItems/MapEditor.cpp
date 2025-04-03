//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapEditor.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"

editor::render::subwindows::MapEditor::MapEditor(editor::Project* project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor") + ""), _project(project) {
    _textures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/toplayer.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedlayer"));
    _textures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/topbottomtp.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.topandbelowtransparent"));
    _textures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/currentandbelow.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedandbelowtransparent"));
    _textures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/alllayers.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.alllayers"));
}

editor::render::subwindows::MapEditor::~MapEditor() {
    for(int i = 0; i < _textures.size(); ++i) {
        RenderManager::GetInstance().destroyTexture(_textures[i]);
    }
    _textures.clear();
}


void editor::render::subwindows::MapEditor::beforeRender() {

}

void editor::render::subwindows::MapEditor::onRender() {
    drawToolbar();
    drawGrid();
}

void editor::render::subwindows::MapEditor::drawGrid() {
    const int* dimensions = _project->getDimensions();
    ImVec2 cursorStart = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for(int j = 0; j < dimensions[1]; ++j) {
        for(int i = 0; i < dimensions[0]; ++i) {
            ImVec2 tilePos = ImVec2(cursorStart.x + i * _tileSize, cursorStart.y + j * _tileSize);
            std::string buttonID = "tile_" + std::to_string(i) + "_" + std::to_string(j);
            ImGui::SetCursorPos(tilePos);
            if(ImGui::InvisibleButton(buttonID.c_str(), ImVec2(_tileSize, _tileSize))) {

            }

            //drawList->AddImage();
            drawList->AddRect(tilePos, ImVec2(tilePos.x + _tileSize, tilePos.y + _tileSize), IM_COL32(255, 255, 255, 50));
        }
    }
}

void editor::render::subwindows::MapEditor::drawToolbar() {
    ImGui::BeginChild("##mapEditorToolbar", ImVec2(render::RenderManager::GetInstance().getWidth(), 60), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6,6));

    for(int i = 0; i < 4; ++i) {
        if(i > 0) ImGui::SameLine();

        if(ImGui::ImageButton(std::string("but" + std::to_string(i)).c_str(), _textures[i], ImVec2(32, 32))) {
            _selectedMode = i;
        }
        if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip(_buttonTooltips[i].c_str());
        }
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
}