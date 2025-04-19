//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapConnections.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include <imgui_internal.h>
#include <resources/Tile.h>

#include "resources/Map.h"
#include <SDL3/SDL_filesystem.h>

editor::render::tabs::MapConnections::MapConnections(editor::Project* project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapconnections") + ""), _project(project) {
    _worlds = {std::vector<resources::Map *>()};
    for (auto [k,map] : _project->getMaps()) {
        int world = map->getWorld();
        if (world >= 0) {
            if (world >= _worlds.size()) _worlds.resize(world + 1);
            _worlds[world].push_back(map);
        }
    }
    _activeWorld = 0;
}

editor::render::tabs::MapConnections::~MapConnections() {
}


void editor::render::tabs::MapConnections::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::MapConnections::onRender() {
    drawSelector();
    ImGui::SameLine();
    drawGrid();
}

void editor::render::tabs::MapConnections::drawGrid() {
    const int* dimensions = _project->getDimensions();
    int gridWidth = 2048, gridHeight = 2048;

    float scaledSize = dimensions[0] * _zoom;
    float contentWidth = gridWidth * scaledSize;
    float contentHeight = gridHeight * scaledSize;

    ImGui::SetNextWindowContentSize(ImVec2(contentWidth, contentHeight));
    ImGui::BeginChild("##mapGrid", ImVec2(0,0), 0, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_HorizontalScrollbar);
    {
        ImVec2 windowSize = ImGui::GetWindowSize();
        if (!_createCentered) {
            float scrollX = (contentWidth - windowSize.x) / 2.0f;
            float scrollY = (contentHeight - windowSize.y) / 2.0f;
            scrollX = std::clamp(scrollX, 0.0f, contentWidth - windowSize.x);
            scrollY = std::clamp(scrollY, 0.0f, contentHeight - windowSize.y);
            ImGui::SetScrollX(scrollX);
            ImGui::SetScrollY(scrollY);
            _createCentered = true;
        }
        zoomAndPan();

        ImVec2 gridStartPos = ImGui::GetCursorScreenPos();
        handleMapDrag(gridWidth, gridHeight, scaledSize, gridStartPos);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        float scrollX = ImGui::GetScrollX();
        float scrollY = ImGui::GetScrollY();
        int firstCol = std::max(0, static_cast<int>(scrollX / scaledSize));
        int lastCol = std::min(gridWidth, static_cast<int>((scrollX + windowSize.x) / scaledSize) + 1);
        int firstRow = std::max(0, static_cast<int>(scrollY / scaledSize));
        int lastRow = std::min(gridHeight, static_cast<int>((scrollY + windowSize.y) / scaledSize) + 1);

        for (int j = firstRow; j < lastRow; ++j) {
            for (int i = firstCol; i < lastCol; ++i) {
                ImVec2 tilePos = {
                    gridStartPos.x + i * scaledSize ,
                    gridStartPos.y + j * scaledSize
                };
                ImVec2 tileEnd = {tilePos.x + scaledSize, tilePos.y + scaledSize};
                if (!drawTileInGrid(i - gridWidth/2, j - gridHeight/2, tilePos, tileEnd, drawList))
                    drawList->AddRect(tilePos, tileEnd, IM_COL32(255, 255, 255, 64));
            }
        }
        if(_somethingModified && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
            save();
        }
        ImGui::EndChild();
    }
}

void editor::render::tabs::MapConnections::drawSelector() {
    ImGui::BeginChild("##worldSelector", ImVec2(RenderManager::GetInstance().getWidth()/6, 0), true);
    {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        std::string world = io::LocalizationManager::GetInstance().getString("window.mainwindow.mapconnections.world");
        if (ImGui::BeginCombo("##worldDropdown", (world + " " +  std::to_string(_activeWorld)).c_str())) {
            for (int i = 0; i < _worlds.size(); i++) {
                bool isSelected = (_activeWorld == i);
                if(ImGui::Selectable((world + " " +  std::to_string(i)).c_str(), isSelected)) {
                    _activeWorld = i;
                }
            }
            ImGui::Separator();
            if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapconnections.createWorld").c_str())) {
                _activeWorld = _worlds.size();
                _worlds.push_back(std::vector<resources::Map *>());
            }
            ImGui::EndCombo();
        }
        ImGui::BeginChild("##mapConnectionsMaps");
        {
            for (auto map = _worlds[_activeWorld].begin(); map != _worlds[_activeWorld].end();) {
                ImGui::BeginChild(("##mapConnections" + (*map)->getName()).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 200), true, ImGuiChildFlags_AutoResizeY);
                {
                    ImGui::Text("%s", (*map)->getName().c_str());
                    ImGui::SameLine();
                    bool eraseMap = ImGui::Button(io::LocalizationManager::GetInstance().getString("window.mainwindow.mapconnections.removeMap").c_str());
                    ImGui::Separator();
                    int pos[2] = { (*map)->getMapX(),(*map)->getMapY()};
                    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.mapconnections.mapPosition").c_str());
                    if (ImGui::InputInt2(("##mapPosition" + (*map)->getName()).c_str(), pos)) {
                        (*map)->setMapX(pos[0]);
                        (*map)->setMapY(pos[1]);
                        _somethingModified = true;
                    }
                    int size[2] = { (*map)->getMapWidth(), (*map)->getMapHeight() };
                    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.dimensions").c_str());
                    ImGui::InputInt2(("##mapSize" + (*map)->getName()).c_str(), size, ImGuiInputTextFlags_ReadOnly);
                    if (eraseMap) {
                        (*map)->setWorld(-1);
                        (*map)->setMapX(0);
                        (*map)->setMapY(0);
                        map = _worlds[_activeWorld].erase(map);
                        _somethingModified = true;
                    }
                    else ++map;
                }
                ImGui::EndChild();
            }
            ImGui::Spacing();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 20);
            if (ImGui::BeginCombo("##addMapDropdown",io::LocalizationManager::GetInstance().getString("window.mainwindow.mapconnections.addmap").c_str())) {
                for (auto [name, map] : _project->getMaps()) {
                    if (map->getWorld() < 0) {
                        if(ImGui::Selectable((name).c_str(), false)) {
                            if (findFreePosition(map)) {
                                _worlds[_activeWorld].push_back(map);
                                map->setWorld(_activeWorld);
                                _somethingModified = true;
                            }
                            else {
                                map->setMapX(0);
                                map->setMapY(0);
                            }
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}

void editor::render::tabs::MapConnections::handleMapDrag(int gridWidth, int gridHeight, float scaledSize, ImVec2 gridStartPos) {
    if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) return;

    ImGuiIO& io = ImGui::GetIO();

    float localX = io.MousePos.x - gridStartPos.x;
    float localY = io.MousePos.y - gridStartPos.y;
    int tileI = static_cast<int>(std::floor(localX / scaledSize));
    int tileJ = static_cast<int>(std::floor(localY / scaledSize));
    int clickGridX = tileI - gridWidth/2;
    int clickGridY = tileJ - gridHeight/2;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        for (auto map : _worlds[_activeWorld]) {
            if (clickGridX >= map->getMapX() && clickGridX < map->getMapX() + map->getMapWidth() &&
                clickGridY >= map->getMapY() && clickGridY < map->getMapY() + map->getMapHeight()) {
                _isDragging = true;
                _draggedMap = map;
                _dragOffsetX = clickGridX - map->getMapX();
                _dragOffsetY = clickGridY - map->getMapY();
                _oldMapX = _draggedMap->getMapX();
                _oldMapY = _draggedMap->getMapY();
                break;
            }
        }
    }

    else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        if (_isDragging && _draggedMap) {
            bool validPosition = true;
            for (const auto& map : _worlds[_activeWorld]) {
                if (map != _draggedMap && _draggedMap->isOverlapping(map)) {
                    validPosition = false;
                    break;
                }
            }
            if (!validPosition) {
                _draggedMap->setMapX(_oldMapX);
                _draggedMap->setMapY(_oldMapY);
            } else {
                _somethingModified = true;
            }
        }
        _isDragging = false;
        _draggedMap = nullptr;
    }

    if (_isDragging && _draggedMap) {
        _draggedMap->setMapX(clickGridX - _dragOffsetX);
        _draggedMap->setMapY(clickGridY - _dragOffsetY);
    }
}


void editor::render::tabs::MapConnections::zoomAndPan() {
    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
        float wheel = io.MouseWheel;
        if (wheel != 0.0f) {
            float newZoom = _zoom * (1.0f + wheel * 0.1f);
            newZoom = std::clamp(newZoom, 0.5f, 1.0f);
            if (newZoom != _zoom) {
                ImVec2 winSize = ImGui::GetWindowSize();
                ImVec2 centerLocal(winSize.x * 0.5f, winSize.y * 0.5f);
                ImVec2 scrollBefore(ImGui::GetScrollX(), ImGui::GetScrollY());
                ImVec2 contentCenterBefore(
                    scrollBefore.x + centerLocal.x,
                    scrollBefore.y + centerLocal.y
                );
                float scale = newZoom / _zoom;
                _zoom = newZoom;
                ImVec2 contentCenterAfter(
                    contentCenterBefore.x * scale,
                    contentCenterBefore.y * scale
                );
                ImGui::SetScrollX(contentCenterAfter.x - centerLocal.x);
                ImGui::SetScrollY(contentCenterAfter.y - centerLocal.y);
            }
        }
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
            _isPanning = true;
            _lastMousePos = io.MousePos;
        } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
            _isPanning = false;
        }
        if (_isPanning) {
            ImVec2 delta = ImVec2(io.MousePos.x - _lastMousePos.x, io.MousePos.y - _lastMousePos.y);
            ImGui::SetScrollX(ImGui::GetScrollX() - delta.x);
            ImGui::SetScrollY(ImGui::GetScrollY() - delta.y);
            _lastMousePos = io.MousePos;
        }
    }
}

bool editor::render::tabs::MapConnections::drawTileInGrid(int gridPosX, int gridPosY, ImVec2 tilePos, ImVec2 tileEnd, ImDrawList *drawList) {
    for (const auto& map : _worlds[_activeWorld]) {
        if (gridPosX >= map->getMapX() && gridPosX < map->getMapX() + map->getMapWidth() &&
            gridPosY >= map->getMapY() && gridPosY < map->getMapY() + map->getMapHeight()) {
            int i = gridPosX - map->getMapX();
            int j = gridPosY - map->getMapY();
            bool drawn = false;
            for(int x = map->getLayers() - 1; x >= 0; --x) {
                resources::Tile* tile = map->getTiles()[x][i + map->getMapWidth() * j];
                if(tile != nullptr) {
                    drawList->AddImage(tile->texture, tilePos, tileEnd, tile->rect.Min, tile->rect.Max);
                    drawn = true;
                }
            }
            if(!drawn) {
                drawList->AddRectFilled(tilePos, tileEnd, IM_COL32(255, 255, 255, 128));
            }
            return true;
        }
    }
    return false;
}

bool editor::render::tabs::MapConnections::findFreePosition(resources::Map* map, int maxRadius) {
    for (int dist = 0; dist <= maxRadius; ++dist) {
        for (int x = -dist; x <= dist; ++x) {
            int y = dist - std::abs(x);
            std::pair<int, int> positions[] = {
                {x, y},
                {x, -y}
            };
            int numPositions = (y == 0) ? 1 : 2;
            for (int i = 0; i < numPositions; ++i) {
                int px = positions[i].first;
                int py = positions[i].second;
                map->setMapX(px);
                map->setMapY(py);
                bool overlaps = false;
                for (auto& placedMap : _worlds[_activeWorld]) {
                    if (map->isOverlapping(placedMap)) {
                        overlaps = true;
                        break;
                    }
                }
                if (!overlaps) {
                    return true;
                }
            }
        }
    }
    return false;
}

void editor::render::tabs::MapConnections::getAdjacentMaps() {
    for (const auto& [key,map] : _project->getMaps()) {
        map->clearAdjacent();
    }
    for (auto& world : _worlds) {
        for (size_t j = 0; j < world.size(); ++j) {
            auto* mapA = world[j];
            for (size_t k = j + 1; k < world.size(); ++k) {
                auto* mapB = world[k];
                if (mapA->isAdjacent(mapB)) {
                    mapA->addAdjacent(mapB->getName());
                    mapB->addAdjacent(mapA->getName());
                }
            }
        }
    }
}

void editor::render::tabs::MapConnections::save() {
    _somethingModified = false;
    _project->saveEverything();
}
