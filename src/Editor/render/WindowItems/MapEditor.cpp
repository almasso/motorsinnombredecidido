//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapEditor.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"
#include <imgui_internal.h>

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
    _textures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/zoomin.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.zoomin"));
    _textures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/zoomout.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.zoomout"));
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
    float scaledSize = _tileSize * _zoom;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 60);

    ImGui::SetNextWindowContentSize(ImVec2(dimensions[0] * scaledSize, dimensions[1] * scaledSize));
    ImGui::BeginChild("##mapGrid", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar);

    float scrollX = ImGui::GetScrollX();
    float scrollY = ImGui::GetScrollY();

    for(int j = 0; j < dimensions[1]; ++j) {
        for(int i = 0; i < dimensions[0]; ++i) {
            ImVec2 tilePos = {
                    cursorStart.x + i * scaledSize + 500 - scrollX,
                    cursorStart.y + j * scaledSize - scrollY
            };
            ImVec2 tileEnd = {tilePos.x + scaledSize, tilePos.y + scaledSize};

            if(tilePos.y < 100) continue;
            if(tilePos.x > 1500) continue;

            std::string buttonID = "tile_" + std::to_string(i) + "_" + std::to_string(j);
            ImGui::SetCursorPos(tilePos);
            if(ImGui::InvisibleButton(buttonID.c_str(), ImVec2(scaledSize, scaledSize))) {

            }

            //drawList->AddImage();
            drawList->AddRect(tilePos, tileEnd, IM_COL32(255, 255, 255, 50));
        }
    }
    ImGui::EndChild();
}

void editor::render::subwindows::MapEditor::drawToolbar() {
    ImGui::BeginChild("##mapEditorToolbar", ImVec2(render::RenderManager::GetInstance().getWidth(), 60), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6,6));

    // Botones de layers
    for(int i = 0; i < 4; ++i) {
        if(i > 0) ImGui::SameLine();

        if(ImGui::ImageButton(std::string("but" + std::to_string(i)).c_str(), _textures[i], ImVec2(32, 32))) {
            _selectedMode = i;
        }
        if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip(_buttonTooltips[i].c_str());
        }
    }

    ImGui::SameLine();
    if(ImGui::ImageButton("butZoomin", _textures[4], ImVec2(32, 32))) {
        _zoom *= 1.1f;
        _zoom = ImClamp(_zoom, 0.5f, 3.0f);
    }
    if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_buttonTooltips[4].c_str());
    }
    ImGui::SameLine();
    if(ImGui::ImageButton("butZoomout", _textures[5], ImVec2(32, 32))) {
        _zoom *= 1.0f/1.1f;
        _zoom = ImClamp(_zoom, 0.5f, 3.0f);
    }
    if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_buttonTooltips[5].c_str());
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
}