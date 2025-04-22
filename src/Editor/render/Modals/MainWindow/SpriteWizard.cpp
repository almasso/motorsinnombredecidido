//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "SpriteWizard.h"
#include "io/LocalizationManager.h"
#include "render/RenderManager.h"
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include "resources/Sprite.h"
#include "common/Project.h"
#include "common/EditorError.h"
#include <SDL3/SDL.h>

editor::render::modals::SpriteWizard::SpriteWizard(editor::Project* project) : ModalWindow("##spriteWizard"), _project(project) {}

editor::render::modals::SpriteWizard::~SpriteWizard() {
    if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
    _loadedTexture = 0;
}

void editor::render::modals::SpriteWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::SpriteWizard::onRender() {
    drawControls();
    ImGui::SameLine();
    drawSprite();
}

void editor::render::modals::SpriteWizard::drawControls() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, _spriteToModify->getName() != "" ? _spriteToModify->getName().c_str() : io::LocalizationManager::GetInstance().getString("sprite.default").c_str(),
                sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
        strncpy(_routeBuffer, _spriteToModify->getSource() != "" ? _spriteToModify->getSource().string().c_str() : "", sizeof(_routeBuffer) - 1);
        _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
        _coords[0] = _spriteToModify->getX();
        _coords[1] = _spriteToModify->getY();
        _dimensions[0] = _spriteToModify->getWidth();
        _dimensions[1] = _spriteToModify->getHeight();
        _maxX = INT_MAX;
        _maxY = INT_MAX;
        _maxWidth = INT_MAX;
        _maxHeight = INT_MAX;
        if(_spriteToModify->getSource() != "") {
            _loadedTexture = RenderManager::GetInstance().loadTexture(_spriteToModify->getSource().string());
            _maxX = ((SDL_Texture*)_loadedTexture)->w - 1;
            _maxY = ((SDL_Texture*)_loadedTexture)->h - 1;
            _maxWidth = ((SDL_Texture*)_loadedTexture)->w;
            _maxHeight = ((SDL_Texture*)_loadedTexture)->h;
        }
    }
    _isGivingName = true;

    ImGui::BeginChild("##spriteControls", ImVec2(600, 720));
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("action.search").c_str())) {
        const char* fileExtension[] = {"*.stb", "*.bmp", "*.gif", "*.jpg", "*.lbm", "*.pcx", "*.png", "*.pnm", "*.qoi", "*.svg", "*.tga", "*.xcf", "*.xpm", "*.xv"};
        const char* route = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("action.selectimage").c_str(),
                (_project->getPath() / "assets/").string().c_str(),
                14,
                fileExtension,
                nullptr,
                0
        );
        if(route != nullptr) {
            if(std::filesystem::path(route).parent_path() != (_project->getPath() / "assets")) {
                showUserWarning(io::LocalizationManager::GetInstance().getString("error.assetlocationnotvalid"))
                strncpy(_routeBuffer, "", sizeof(_routeBuffer) - 1);
                _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
            }
            else {
                if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
                std::string fR = std::string(route);
                fR = std::filesystem::path(fR).lexically_normal().string();
                strncpy(_routeBuffer, fR.c_str(), sizeof(_routeBuffer) - 1);
                _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
                _loadedTexture = RenderManager::GetInstance().loadTexture(fR);
            }
        }
    }
    ImGui::SameLine();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.spritewizard.source").c_str(),
                     _routeBuffer, IM_ARRAYSIZE(_routeBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::Spacing();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.spritewizard.spritename").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    if(_project->getTileset(_nameBuffer) != nullptr) _sameName = true;
    else _sameName = false;

    if(_spriteToModify->getName() != "" && _spriteToModify->getName() == _nameBuffer) _sameName = false;

    if(_sameName) {
        ImGui::TextColored(ImColor(255,0,0),io::LocalizationManager::GetInstance().getString("error.samespritename").c_str());
    }

    ImGui::Spacing();
    ImGui::SliderInt(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.dimensionx").c_str(),
                      &_coords[0], 0, _maxX);
    ImGui::Spacing();
    ImGui::SliderInt(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.dimensiony").c_str(),
                      &_coords[1], 0, _maxY);
    ImGui::Spacing();
    ImGui::SliderInt(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.dimensionw").c_str(),
                      &_dimensions[0], 1, _maxWidth);
    ImGui::Spacing();
    ImGui::SliderInt(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.dimensionh").c_str(),
                      &_dimensions[1], 1, _maxHeight);

    _maxWidth = ((SDL_Texture*)_loadedTexture)->w - _coords[0];
    _maxHeight = ((SDL_Texture*)_loadedTexture)->h - _coords[1];


    ImGui::SetCursorPosY(ImGui::GetCursorPosX() + 390);
    ImGui::Spacing();
    ImGui::ColorEdit3(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.spritewizard.gridcolor").c_str(),
                      _color);

    ImGui::SetCursorPosY(ImGui::GetCursorPosX() + 460);

    ImGui::BeginDisabled(std::string(_routeBuffer) == "" || _sameName);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString(_modify ? "action.edit" : "action.add").c_str(), ImVec2(120, 0))) {
        _spriteToModify->init(_nameBuffer, _routeBuffer, _coords[0], _coords[1], _dimensions[0], _dimensions[1]);
        ImGui::CloseCurrentPopup();
        _spriteToModify = nullptr;
        _isOpen = false;
        _isGivingName = false;
        if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
        _loadedTexture = 0;
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _spriteToModify = nullptr;
        _isOpen = false;
        _isGivingName = false;
        if(_loadedTexture != 0) RenderManager::GetInstance().destroyTexture(_loadedTexture);
        _loadedTexture = 0;
    }
    ImGui::EndChild();
}

void editor::render::modals::SpriteWizard::drawSprite() {
    ImGui::BeginChild("##spritePreview", ImVec2(720, 720), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
    {
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        int imageWidth = 200, imageHeight = 200;
        if(_loadedTexture != 0) imageWidth = ((SDL_Texture*)_loadedTexture)->w, imageHeight = ((SDL_Texture*)_loadedTexture)->h;

        ImVec2 imageSize = ImVec2(imageWidth, imageHeight);
        ImGui::Image(_loadedTexture, imageSize);

        drawList->AddRect(ImVec2(_coords[0], _coords[1]), ImVec2(_coords[0] + _dimensions[0], _coords[1] + _dimensions[1]), IM_COL32(_color[0] * 255, _color[1] * 255, _color[2] * 255, 128));
    }
    ImGui::EndChild();
}

void editor::render::modals::SpriteWizard::setSpriteToModify(editor::resources::Sprite* sprite, bool modify) {
    _modify = modify;
    _spriteToModify = sprite;
}


