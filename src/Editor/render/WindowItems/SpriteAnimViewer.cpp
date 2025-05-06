//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "SpriteAnimViewer.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include <imgui_internal.h>
#include "resources/Sprite.h"
#include "resources/Animation.h"
#include "render/Modals/MainWindow/SpriteWizard.h"
#include "render/Modals/MainWindow/AnimationWizard.h"
#include "render/WindowStack.h"
#include <SDL3/SDL.h>

editor::render::tabs::SpriteAnimViewer::SpriteAnimViewer(editor::Project *project) :
WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.spriteeditor") + ""), _project(project) {
    _spriteWizard = new render::modals::SpriteWizard(project);
    WindowStack::addWindowToStack(_spriteWizard);
    _animationWizard = new render::modals::AnimationWizard(project);
    WindowStack::addWindowToStack(_animationWizard);
}

editor::render::tabs::SpriteAnimViewer::~SpriteAnimViewer() {
    WindowStack::removeWindowFromStack(_spriteWizard);
    WindowStack::removeWindowFromStack(_animationWizard);
    delete _spriteWizard;
    _spriteWizard = nullptr;
    delete _animationWizard;
    _animationWizard = nullptr;
}

void editor::render::tabs::SpriteAnimViewer::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::SpriteAnimViewer::onRender() {
    if(ImGui::BeginTabBar("animspritetabbar")) {
        drawSpriteGrid();
        drawAnimGrid();
        ImGui::EndTabBar();
    }
}

void editor::render::tabs::SpriteAnimViewer::save() {

}

void editor::render::tabs::SpriteAnimViewer::drawSpriteGrid() {
    if(ImGui::BeginTabItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.spriteeditor.sprites").c_str())) {
        const float thumbnailSize = 250.0f;
        const float padding = 8.0f;
        float cellSize = thumbnailSize + padding;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = (int)(panelWidth/cellSize);
        if(columns < 1) columns = 1;

        int count = 0;
        ImGui::Dummy(ImVec2(0, 50.0f));
        ImGui::Indent(50.0f);
        for(auto it = _project->getSprites().begin(); it != _project->getSprites().end();) {
            bool erased = false;
            ImGui::PushID(count);
            auto* sprite = it->second;

            float textureWidth = ((SDL_Texture*)sprite->getTextureID())->w;
            float textureHeight = ((SDL_Texture*)sprite->getTextureID())->h;

            ImVec2 uv0 = ImVec2(sprite->getX() / textureWidth, sprite->getY() / textureHeight);
            ImVec2 uv1 = ImVec2((sprite->getX() + sprite->getWidth()) / textureWidth, (sprite->getY() + sprite->getHeight()) / textureHeight);

            ImGui::BeginGroup();
            ImGui::Image(it->second->getTextureID(), ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);
            float textWidth = ImGui::CalcTextSize(it->second->getName().c_str()).x;
            float textOffsetX = (thumbnailSize - textWidth) * 0.5f;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffsetX);
            ImGui::Text("%s", it->second->getName().c_str());
            ImGui::EndGroup();
            ImGui::PopID();
            if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup((it->second->getName() + "spriteoptions").c_str());
            }
            if(ImGui::BeginPopupContextItem((it->second->getName() + "spriteoptions").c_str())) {
                if(ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.edit").c_str())) {
                    _spriteWizard->setSpriteToModify(it->second, true);
                    _spriteWizard->show();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                if(ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.delete").c_str())) {
                    resources::Sprite* spriteTmp = it->second;
                    it = _project->removeSprite(spriteTmp->getName());
                    erased = true;
                    delete spriteTmp;
                    spriteTmp = nullptr;
                    _somethingModified = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::PopStyleColor();
                ImGui::EndPopup();
            }
            ++count;
            if(!erased) ++it;
            if(count % columns != 0) ImGui::SameLine();
        }

        if(count % columns != 0) ImGui::SameLine();
        ImGui::PushID("addSprite");
        if(ImGui::Button("+", ImVec2(thumbnailSize, thumbnailSize))) {
            _createdSprite = new editor::resources::Sprite(_project);
            _spriteWizard->setSpriteToModify(_createdSprite);
            _spriteWizard->show();
        }
        ImGui::PopID();
        ImGui::Unindent(50.0f);

        ImGui::EndTabItem();
    }

    if(_spriteOpened && !_spriteWizard->isOpen()) {
        if(_createdSprite != nullptr) {
            if(_createdSprite->isInitialized()) {
                _project->addSprite(_createdSprite);
                _createdSprite = nullptr;
                _somethingModified = true;
            }
            else {
                delete _createdSprite;
                _createdSprite = nullptr;
            }
            _spriteOpened = false;
        }
        else {
            _spriteOpened = false;
            _project->refreshSprites();
            _somethingModified = true;
        }
    }

    if(_spriteWizard->hasBeenCalled()) _spriteOpened = true;
}

void editor::render::tabs::SpriteAnimViewer::drawAnimGrid() {
    if(ImGui::BeginTabItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.spriteeditor.animations").c_str())) {
        ImGui::EndTabItem();
    }
}
