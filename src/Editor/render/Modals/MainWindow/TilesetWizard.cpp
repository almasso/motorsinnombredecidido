//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "TilesetWizard.h"
#include "io/LocalizationManager.h"
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include "render/RenderManager.h"
#include <SDL3/SDL.h>

editor::render::modals::TilesetWizard::TilesetWizard(const int* dimensions) :
ModalWindow(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.title") + "") {
    _dimensions[0] = dimensions[0];
    _dimensions[1] = dimensions[1];
    _color[0] = _color[1] = _color[2] = 1;
    _offset[0] = _offset[1] = 0;
}

editor::render::modals::TilesetWizard::~TilesetWizard() noexcept {
    if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
    _loadedTexture = 0;
}

void editor::render::modals::TilesetWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::TilesetWizard::onRender() {
    drawControls();
    ImGui::SameLine();
    drawGrid();
}

void editor::render::modals::TilesetWizard::drawControls() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, io::LocalizationManager::GetInstance().getString("tileset.default").c_str(), sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
        strncpy(_routeBuffer, "", sizeof(_routeBuffer) - 1);
        _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
    }
    _isGivingName = true;

    ImGui::BeginChild("##tilesetControls", ImVec2(600, 512));
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("action.search").c_str())) {
        const char* fileExtension[] = {"*.stb", "*.bmp", "*.gif", "*.jpg", "*.lbm", "*.pcx", "*.png", "*.pnm", "*.qoi", "*.svg", "*.tga", "*.xcf", "*.xpm", "*.xv"};
        const char* route = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("action.selectimage").c_str(),
                "",
                14,
                fileExtension,
                nullptr,
                0
        );
        if(route != nullptr) {
            if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
            std::string fR = std::string(route);
            fR = std::filesystem::path(fR).lexically_normal().string();
            strncpy(_routeBuffer, fR.c_str(), sizeof(_routeBuffer) - 1);
            _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
            _loadedTexture = RenderManager::GetInstance().loadTexture(fR);
        }
    }
    ImGui::SameLine();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.source").c_str(),
                     _routeBuffer, IM_ARRAYSIZE(_routeBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::Spacing();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.tilename").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::Spacing();
    ImGui::SliderInt2(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.offset").c_str(),
                      _offset, -_dimensions[0], _dimensions[0]);


    ImGui::SetCursorPosY(ImGui::GetCursorPosX() + 390);
    ImGui::Spacing();
    ImGui::ColorEdit3(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.gridcolor").c_str(),
                      _color);

    ImGui::SetCursorPosY(ImGui::GetCursorPosX() + 460);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("action.addtileset").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isGivingName = false;
        if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
        _loadedTexture = 0;
        _offset[0] = _offset[1] = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isGivingName = false;
        if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
        _loadedTexture = 0;
        _offset[0] = _offset[1] = 0;
    }
    ImGui::EndChild();
}

void editor::render::modals::TilesetWizard::drawGrid() {
    ImGui::BeginChild("##tilesetPreview", ImVec2(512, 512), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
    {
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        int imageWidth = 200, imageHeight = 200;
        if(_loadedTexture != 0) imageWidth = ((SDL_Texture*)_loadedTexture)->w, imageHeight = ((SDL_Texture*)_loadedTexture)->h;

        ImVec2 imageSize = ImVec2(imageWidth, imageHeight);
        ImGui::Image(_loadedTexture, imageSize);

        _offset[0] = (_offset[0] % _dimensions[0] + _dimensions[0]) % _dimensions[0];
        _offset[1] = (_offset[1] % _dimensions[1] + _dimensions[1]) % _dimensions[1];
        for(int i = _offset[0]; i < imageWidth; i += _dimensions[0]) {
            drawList->AddLine(ImVec2(cursorPos.x + i, cursorPos.y), ImVec2(cursorPos.x + i, cursorPos.y + imageHeight), IM_COL32(_color[0] * 255, _color[1] * 255, _color[2] * 255, 128));
        }
        for(int j = _offset[1]; j < imageHeight; j += _dimensions[1]) {
            drawList->AddLine(ImVec2(cursorPos.x, cursorPos.y + j), ImVec2(cursorPos.x + imageWidth, cursorPos.y + j), IM_COL32(_color[0] * 255, _color[1] * 255, _color[2] * 255, 128));
        }
    }
    ImGui::EndChild();
}
