//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapEditor.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include <imgui_internal.h>
#include "resources/Tile.h"
#include "render/Modals/MainWindow/TilesetWizard.h"
#include "render/WindowStack.h"
#include "resources/Map.h"
#include "resources/Tileset.h"
#include "render/Modals/MainWindow/MapWizard.h"
#include <SDL3/SDL_filesystem.h>


#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#else
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif

editor::render::tabs::MapEditor::MapEditor(editor::Project* project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor") + ""), _project(project) {

    _currentDirectory = GetCurrentDir;
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/toplayer.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedlayer"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/currentandbelowtp.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedandbelowtransparent"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/currentandbelow.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.selectedandbelowopaque"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/alllayers.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.alllayers"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/zoomin.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.zoomin"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/zoomout.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.zoomout"));
    _uiTextures.push_back(RenderManager::GetInstance().loadTexture(std::string(_currentDirectory) + "/settings/assets/map/grid.png"));
    _buttonTooltips.push_back(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.grid"));

    _tilesetWizard = new render::modals::TilesetWizard(project);
    WindowStack::addWindowToStack(_tilesetWizard);
    _mapWizard = new render::modals::MapWizard(project);
    WindowStack::addWindowToStack(_mapWizard);
}

editor::render::tabs::MapEditor::~MapEditor() {
    SDL_free(_currentDirectory);
    for(int i = 0; i < _uiTextures.size(); ++i) {
        RenderManager::GetInstance().destroyTexture(_uiTextures[i]);
    }
    _uiTextures.clear();
    WindowStack::removeWindowFromStack(_tilesetWizard);
    WindowStack::removeWindowFromStack(_mapWizard);
    delete _tilesetWizard;
    _tilesetWizard = nullptr;
    delete _mapWizard;
    _mapWizard = nullptr;
}


void editor::render::tabs::MapEditor::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::MapEditor::onRender() {
    drawTileSelector();
    ImGui::SameLine();
    ImGui::BeginChild("Toolbar+Grid");
    drawToolbar();
    drawGrid();
    ImGui::EndChild();
    ImGui::SameLine();
    drawObjectInspector();
}

void editor::render::tabs::MapEditor::drawGrid() {
    const int* dimensions = _project->getDimensions();
    float scaledSize = dimensions[0] * _zoom;

    int mapWidth = 32, mapHeight = 32;
    if(_selectedMap != nullptr) {
        mapWidth = _selectedMap->getMapWidth();
        mapHeight = _selectedMap->getMapHeight();
    }

    ImVec2 cursorStart = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImGui::SetNextWindowContentSize(ImVec2(mapWidth * scaledSize, mapHeight * scaledSize));
    ImGui::BeginChild("##mapGrid", ImVec2(RenderManager::GetInstance().getWidth()/2,0), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
        float scrollX = ImGui::GetScrollX();
        float scrollY = ImGui::GetScrollY();
        float mouseX = ImGui::GetMousePos().x;
        float mouseY = ImGui::GetMousePos().y;

        bool isDragging = ImGui::IsMouseDown(ImGuiMouseButton_Left);

        for (int j = 0; j < mapHeight; ++j) {
            for (int i = 0; i < mapWidth; ++i) {
                ImVec2 tilePos = {
                        cursorStart.x + i * scaledSize - scrollX,
                        cursorStart.y + j * scaledSize - scrollY
                };
                ImVec2 tileEnd = {tilePos.x + scaledSize, tilePos.y + scaledSize};

                if(tilePos.y < 100) continue;
                if(tileEnd.x > 3 * RenderManager::GetInstance().getWidth()/4) continue;

                std::string buttonID = "tile_" + std::to_string(i) + "_" + std::to_string(j);
                ImGui::SetCursorScreenPos(tilePos);
                if (ImGui::InvisibleButton(buttonID.c_str(), ImVec2(scaledSize, scaledSize))) {
                    if(_project->totalTilesets() > 0 && _selectedTileset != nullptr && _selectedMap != nullptr) {
                        _selectedMap->getTiles()[_selectedLayer][i + mapWidth * j] = _selectedTileset->getTiles()[_selectedTile];
                        _somethingModified = true;
                    }
                }

                bool hovered = mouseX >= tilePos.x && mouseX < tileEnd.x && mouseY >= tilePos.y && mouseY < tileEnd.y;

                if(isDragging && hovered) {
                    if(_project->totalTilesets() > 0 && _selectedTileset != nullptr && _selectedMap != nullptr) {
                        _selectedMap->getTiles()[_selectedLayer][i + mapWidth * j] = _selectedTileset->getTiles()[_selectedTile];
                        _somethingModified = true;
                    }
                }

                if(_selectedMap != nullptr) drawTileInGrid(_selectedMap, mapWidth, i, j, tilePos, tileEnd, drawList);
                if (_isGridShown) drawList->AddRect(tilePos, tileEnd, IM_COL32(255, 255, 255, 50));
            }
        }

        if(_somethingModified && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
            _somethingModified = false;
            _project->saveEverything();
        }
    }
    ImGui::EndChild();
}

void editor::render::tabs::MapEditor::drawTileInGrid(resources::Map* currentMap, int mapWidth, int i, int j, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList* drawList) {
    switch(_selectedGridMode) {
        case GridDrawingMode::DRAW_SELECTED_LAYER_ONLY: {
            resources::Tile* tile = currentMap->getTiles()[_selectedLayer][i + mapWidth * j];
            if(tile != nullptr)
                drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
        } break;
        case GridDrawingMode::DRAW_SELECTED_LAYER_BELOW_TRANSPARENT: {
            for(int x = currentMap->getLayers() - 1; x > _selectedLayer; --x) {
                resources::Tile* tile = currentMap->getTiles()[x][i + mapWidth * j];
                if(tile != nullptr)
                    drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
            }
            drawList->AddRectFilled(tilePos, tileEnd, IM_COL32(0, 0, 0, 100));
            resources::Tile* tile = currentMap->getTiles()[_selectedLayer][i + mapWidth * j];
            if(tile != nullptr)
                drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
        } break;
        case GridDrawingMode::DRAW_SELECTED_LAYER_BELOW_OPAQUE: {
            for(int x = currentMap->getLayers() - 1; x > _selectedLayer; --x) {
                resources::Tile* tile = currentMap->getTiles()[x][i + mapWidth * j];
                if(tile != nullptr)
                    drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
            }
            resources::Tile* tile = currentMap->getTiles()[_selectedLayer][i + mapWidth * j];
            if(tile != nullptr)
                drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
        } break;
        case GridDrawingMode::DRAW_ALL: {
            for(int x = currentMap->getLayers() - 1; x >= 0; --x) {
                resources::Tile* tile = currentMap->getTiles()[x][i + mapWidth * j];
                if(tile != nullptr)
                    drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
            }
        } break;
    }
}

void editor::render::tabs::MapEditor::drawToolbar() {
    ImGui::BeginChild("##mapEditorToolbar", ImVec2(render::RenderManager::GetInstance().getWidth() / 2, 60), true, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6,6));

    // Botones de layers
    for(int i = 0; i < 4; ++i) {
        if(i > 0) ImGui::SameLine();
        ImVec4 bg = _selectedGridMode == i ? ImVec4(1,0,1,1) : ImVec4(0,0,0,0);


        if(ImGui::ImageButton(std::string("but" + std::to_string(i)).c_str(), _uiTextures[i], ImVec2(32, 32), ImVec2(0,0), ImVec2(1,1), bg)) {
            _selectedGridMode = i;
        }
        if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip(_buttonTooltips[i].c_str());
        }
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::Spacing();
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
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImVec4 bg = _isGridShown ? ImVec4(1,0,1,1) : ImVec4(0,0,0,0);
    if(ImGui::ImageButton("butGrid", _uiTextures[6], ImVec2(32, 32), ImVec2(0,0), ImVec2(1,1), bg)) {
        _isGridShown = !_isGridShown;
    }
    if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip(_buttonTooltips[6].c_str());
    }

    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::BeginChild("##dropdowns");
    {
        ImGui::SetNextItemWidth(250);
        if (ImGui::BeginCombo("##mapDropdown", _selectedMap != nullptr ? _selectedMap->getName().c_str() : io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.mapselector").c_str())) {
            for(auto map : _project->getMaps()) {
                bool isSelected = (map.second == _selectedMap);
                if(ImGui::Selectable(map.first.c_str(), isSelected)) {
                    _selectedMap = map.second;
                    _selectedLayer = 0;
                }
            }
            if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.createmap").c_str())) {
                _createdMap = new editor::resources::Map(_project);
                _mapWizard->setMapToModify(_createdMap);
                _mapWizard->show();
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(250);
        if (ImGui::BeginCombo("##layerDropdown", _selectedLayer >= 0 ? (io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.layer") + " " + std::to_string(_selectedLayer)).c_str() : io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.layerselector").c_str())) {
            if(_selectedMap != nullptr) {
                for(int i = 0; i < _selectedMap->getLayers(); ++i) {
                    bool isSelected = (i == _selectedLayer);
                    if (ImGui::Selectable((io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.layer") + " " + std::to_string(i)).c_str(), isSelected)) {
                        _selectedLayer = i;
                    }
                }
            }
            if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.createlayer").c_str())) {
                _selectedMap->addLayer();
                _somethingModified = true;
            }
            ImGui::EndCombo();
        }
    }
    ImGui::EndChild();

    ImGui::PopStyleVar();
    ImGui::EndChild();

    if(_mapOpened && !_mapWizard->isOpen()) {
        if(_createdMap->isInitialized()) {
            _project->addMap(_createdMap);
            _createdMap = nullptr;
            _somethingModified = true;
        }
        else {
            delete _createdMap;
            _createdMap = nullptr;
        }
        _mapOpened = false;
    }

    if(_mapWizard->hasBeenCalled()) _mapOpened = true;
}

void editor::render::tabs::MapEditor::drawTileSelector() {
    ImGui::BeginChild("##tileSelector", ImVec2(RenderManager::GetInstance().getWidth()/4, 0), true);
    {
        if (ImGui::BeginCombo("##tilesetDropdown", _selectedTileset != nullptr ? _selectedTileset->getName().c_str() : io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.tilesetselector").c_str())) {
            for(auto tileset : _project->getTilesets()) {
                bool isSelected = (_selectedTileset == tileset.second);
                if(ImGui::Selectable(tileset.second->getName().c_str(), isSelected)) {
                    _selectedTileset = tileset.second;
                }
            }
            if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.createtileset").c_str())) {
                _createdTileset = new editor::resources::Tileset(_project);
                _tilesetWizard->setTilesetToModify(_createdTileset);
                _tilesetWizard->show();
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();

        ImGui::BeginChild("##tilesetGrid", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        if(_selectedTileset != nullptr) {
            int i = 0;
            for(auto tile : _selectedTileset->getTiles()) {
                if(i % 3 != 0) ImGui::SameLine();
                if(ImGui::ImageButton(("tile" + std::to_string(i)).c_str(), tile->texture, ImVec2(32, 32), tile->rect.Min, tile->rect.Max)) {
                    _selectedTile = i;
                }
                ++i;
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();

    if(_tilesetOpened && !_tilesetWizard->isOpen()) {
        if(_createdTileset->isInitialized()) {
            _project->addTileset(_createdTileset);
            _createdTileset = nullptr;
            _somethingModified = true;
        }
        else {
            delete _createdTileset;
            _createdTileset = nullptr;
        }
        _tilesetOpened = false;
    }

    if(_tilesetWizard->hasBeenCalled()) _tilesetOpened = true;
}

void editor::render::tabs::MapEditor::drawObjectInspector() {
    ImGui::BeginChild("##objectInspector", ImVec2(RenderManager::GetInstance().getWidth()/4, 0), true);
    ImGui::EndChild();
}