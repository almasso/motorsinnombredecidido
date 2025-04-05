//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapEditor.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include <imgui_internal.h>
#include <resources/Tile.h>

#include "render/Modals/MainWindow/TilesetWizard.h"
#include "render/WindowStack.h"
#include "resources/Map.h"
#include "resources/Tileset.h"

editor::render::tabs::MapEditor::MapEditor(editor::Project* project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor") + ""), _project(project) {
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/toplayer.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedlayer"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/currentandbelowtp.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedandbelowtransparent"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/currentandbelow.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedandbelowopaque"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/alllayers.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.alllayers"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/zoomin.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.zoomin"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/zoomout.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.zoomout"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture("/settings/assets/map/grid.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.grid"));

    _tilesetWizard = new render::modals::TilesetWizard();
    WindowStack::addWindowToStack(_tilesetWizard);
    _maps = std::deque<editor::resources::Map*>(0, nullptr);
    _tilesets = std::deque<editor::resources::Tileset*>(0, nullptr);
}

editor::render::tabs::MapEditor::~MapEditor() {
    for(int i = 0; i < _uiTextures.size(); ++i) {
        RenderManager::GetInstance().destroyTexture(_uiTextures[i]);
    }
    _uiTextures.clear();
    WindowStack::removeWindowFromStack(_tilesetWizard);
    delete _tilesetWizard;
    _tilesetWizard = nullptr;
}


void editor::render::tabs::MapEditor::beforeRender() {

}

void editor::render::tabs::MapEditor::onRender() {
    drawTileSelector();
    ImGui::SameLine();
    ImGui::BeginChild("Toolbar+Grid");
    drawToolbar();
    drawGrid();
    ImGui::EndChild();
}

void editor::render::tabs::MapEditor::drawGrid() {
    const int* dimensions = _project->getDimensions();
    float scaledSize = dimensions[0] * _zoom;

    resources::Map* currentMap = nullptr;
    if(_maps.size() > 0) currentMap = _maps[_selectedMap];

    int mapWidth = 32, mapHeight = 32;
    if(currentMap != nullptr) {
        mapWidth = currentMap->getMapWidth();
        mapHeight = currentMap->getMapHeight();
    }

    ImVec2 cursorStart = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 60);
    ImGui::SetNextWindowContentSize(ImVec2(mapWidth * scaledSize, mapHeight * scaledSize));
    ImGui::BeginChild("##mapGrid", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
        float scrollX = ImGui::GetScrollX();
        float scrollY = ImGui::GetScrollY();

        bool isDragging = ImGui::IsMouseDown(ImGuiMouseButton_Left);

        for (int j = 0; j < mapHeight; ++j) {
            for (int i = 0; i < mapWidth; ++i) {
                ImVec2 tilePos = {
                        cursorStart.x + i * scaledSize - scrollX,
                        cursorStart.y + j * scaledSize - scrollY
                };
                ImVec2 tileEnd = {tilePos.x + scaledSize, tilePos.y + scaledSize};

                if (tilePos.y < 100) continue;

                std::string buttonID = "tile_" + std::to_string(i) + "_" + std::to_string(j);
                ImGui::SetCursorPos(tilePos);
                if (ImGui::InvisibleButton(buttonID.c_str(), ImVec2(scaledSize, scaledSize))) {
                    if(_tilesets.size() > 0 && _tilesets[_selectedTileset] != nullptr) {
                        currentMap->getTiles()[_selectedLayer][i + mapWidth * j] = _tilesets[_selectedTileset]->getTiles()[_selectedTile];
                    }
                }

                if(isDragging && ImGui::IsItemHovered()) {
                    if(_tilesets.size() > 0 && _tilesets[_selectedTileset] != nullptr) {
                        currentMap->getTiles()[_selectedLayer][i + mapWidth * j] = _tilesets[_selectedTileset]->getTiles()[_selectedTile];
                    }
                }

                if(currentMap != nullptr)
                    drawTileInGrid(currentMap, mapWidth, i, j, tilePos, tileEnd, drawList);
                if (_isGridShown) drawList->AddRect(tilePos, tileEnd, IM_COL32(255, 255, 255, 50));
            }
        }
    }
    ImGui::EndChild();
}

void editor::render::tabs::MapEditor::drawTileInGrid(resources::Map* currentMap, int mapWidth, int i, int j, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList* drawList) {
    switch(_selectedGridMode) {
        case GridDrawingMode::DRAW_SELECTED_LAYER_ONLY: {
            if(currentMap->getTiles()[_selectedLayer][i + mapWidth * j] != nullptr)
                drawList->AddImage(currentMap->getTiles()[_selectedLayer][i + mapWidth * j]->texture, tilePos, tileEnd);
        } break;
        case GridDrawingMode::DRAW_SELECTED_LAYER_BELOW_TRANSPARENT: {
            for(int x = currentMap->getLayers() - 1; x > _selectedLayer; --x) {
                drawList->AddImage(currentMap->getTiles()[x][i + mapWidth * j]->texture, tilePos, tileEnd);
            }
            drawList->AddRectFilled(ImVec2(0,0), ImVec2(1,1), IM_COL32(73, 61, 67, 100));
            drawList->AddImage(currentMap->getTiles()[_selectedLayer][i + mapWidth * j]->texture, tilePos, tileEnd);
        } break;
        case GridDrawingMode::DRAW_SELECTED_LAYER_BELOW_OPAQUE: {
            for(int x = currentMap->getLayers() - 1; x > _selectedLayer; --x) {
                drawList->AddImage(currentMap->getTiles()[x][i + mapWidth * j]->texture, tilePos, tileEnd);
            }
            drawList->AddImage(currentMap->getTiles()[_selectedLayer][i + mapWidth * j]->texture, tilePos, tileEnd);
        } break;
        case GridDrawingMode::DRAW_ALL: {
            for(int x = currentMap->getLayers() - 1; x >= 0; --x) {
                drawList->AddImage(currentMap->getTiles()[x][i + mapWidth * j]->texture, tilePos, tileEnd);
            }
        } break;
    }
}

void editor::render::tabs::MapEditor::drawToolbar() {
    ImGui::BeginChild("##mapEditorToolbar", ImVec2(render::RenderManager::GetInstance().getWidth(), 60), 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6,6));

    // Botones de layers
    for(int i = 0; i < 4; ++i) {
        if(i > 0) ImGui::SameLine();

        if(ImGui::ImageButton(std::string("but" + std::to_string(i)).c_str(), _uiTextures[i], ImVec2(32, 32))) {
            _selectedGridMode = i;
        }
        if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip(_buttonTooltips[i].c_str());
        }
    }

    ImGui::SameLine();
    if(ImGui::ImageButton("butZoomin", _uiTextures[4], ImVec2(32, 32))) {
        _zoom *= 1.1f;
        _zoom = ImClamp(_zoom, 0.5f, 3.0f);
    }
    if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_buttonTooltips[4].c_str());
    }
    ImGui::SameLine();
    if(ImGui::ImageButton("butZoomout", _uiTextures[5], ImVec2(32, 32))) {
        _zoom *= 1.0f/1.1f;
        _zoom = ImClamp(_zoom, 0.5f, 3.0f);
    }
    if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_buttonTooltips[5].c_str());
    }
    ImGui::SameLine();
    if(ImGui::ImageButton("butGrid", _uiTextures[6], ImVec2(32, 32))) {
        _isGridShown = !_isGridShown;
    }
    if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_buttonTooltips[6].c_str());
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void editor::render::tabs::MapEditor::drawTileSelector() {
    ImGui::BeginChild("##tileSelector", ImVec2(500, 0), true);
    {
        int selectedIndex = -1;

        if (ImGui::BeginCombo("Tileset", selectedIndex >= 0 ? ("Tileset " + std::to_string(selectedIndex)).c_str() : "Select tileset")) {
            for (int i = 0; i < _tilesets.size(); ++i) {
                bool isSelected = (i == selectedIndex);
                if (ImGui::Selectable(("Tileset " + std::to_string(i)).c_str(), isSelected)) {
                    selectedIndex = i;
                    _selectedTileset = i;
                }
            }
            if (ImGui::Selectable("+ Create new tileset")) {

            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        ImGui::BeginChild("##tilesetGrid", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        if (_selectedTileset >= 0 && _selectedTileset < _tilesets.size()) {
            /*for (int i = 0; i < 64; ++i) {
                if (i % 3 != 0) ImGui::SameLine();
                if (ImGui::ImageButton(("tile" + std::to_string(i)).c_str(), _tilesets[_selectedTileset], ImVec2(32, 32))) {
                    _selectedTileID = i;
                }
            }*/
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}