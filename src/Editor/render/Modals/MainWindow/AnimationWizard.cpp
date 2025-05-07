//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//


#include "AnimationWizard.h"
#include "resources/Animation.h"
#include <SDl3/SDL.h>
#include "io/LocalizationManager.h"
#include "render/RenderManager.h"
#include "common/Project.h"
#include "utils/IconsFontAwesome6.h"
#include "resources/Sprite.h"

editor::render::modals::AnimationWizard::AnimationWizard(editor::Project *project) : ModalWindow(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.animationwizard.title") + ""), _project(project) {}

editor::render::modals::AnimationWizard::~AnimationWizard() = default;

void editor::render::modals::AnimationWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::AnimationWizard::onRender() {
    ImGui::BeginChild("##leftPart", ImVec2(800, 650), true);
    drawControls();
    drawThumbnail();

    ImGui::BeginDisabled(_frames.empty() || _sameName);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString(_modify ? "action.edit" : "action.add").c_str(), ImVec2(120, 0))) {
        _animationToModify->init(_nameBuffer, getSpritesVectorFromList(), _timeBetweenFrames, _loop);
        ImGui::CloseCurrentPopup();
        _animationToModify = nullptr;
        _isOpen = false;
        _isGivingName = false;
        _frames.clear();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _animationToModify = nullptr;
        _isOpen = false;
        _isGivingName = false;
        _frames.clear();
    }

    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##rightPart", ImVec2(420, 650), true);
    drawAnimation();
    drawAnimationControls();
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::drawControls() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, _animationToModify->getName() != "" ? _animationToModify->getName().c_str() : io::LocalizationManager::GetInstance().getString("animation.default").c_str(),
                sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
        if(_animationToModify->isInitialized()) {
            _loop = _animationToModify->isLoop();
            _timeBetweenFrames = _animationToModify->getTimeBetweenFrames();
            getSpritesListFromVector(_animationToModify->getFrames());
        }
    }
    _isGivingName = true;

    ImGui::BeginChild("#animationControls", ImVec2(750, 150));
    {
        ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.animationwizard.animationame").c_str(),
                 _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

        if(_project->getAnimation(_nameBuffer) != nullptr) _sameName = true;
        else _sameName = false;

        if(_animationToModify->getName() != "" && _animationToModify->getName() == _nameBuffer) _sameName = false;

        if(_sameName) {
            ImGui::TextColored(ImColor(255,0,0),io::LocalizationManager::GetInstance().getString("error.sameanimationname").c_str());
        }

        ImGui::Spacing();
        ImGui::DragFloat(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.animationwizard.timebetweenframes").c_str(),
            &_timeBetweenFrames, 0.1f, 0.1f, 5.0f);
        ImGui::Spacing();
        ImGui::Checkbox(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.animationwizard.loop").c_str(), &_loop);

        ImGui::Spacing();
    }
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::drawThumbnail() {
    ImGui::BeginChild("##thumbnails", ImVec2(300,400), true);
    ImVec2 thumbnailSize(64, 64);
    int count = 0;
    for(auto it = _frames.begin(); it != _frames.end(); ++it, ++count) {
        ImGui::PushID(count);
        ImGui::BeginGroup();

        ImGui::BeginGroup();

        ImVec2 p0 = ImGui::GetCursorScreenPos();
        float grabLineHeight = 2.0f;
        float grabLineSpacing = 4.0f;
        int grabLineCount = 3;
        float grabLineWidth = 10.0f;
        ImU32 grabColor = ImGui::GetColorU32(ImGuiCol_TextDisabled);

        float totalGrabHeight = grabLineCount * grabLineSpacing;
        float startY = p0.y + (thumbnailSize.y - totalGrabHeight) * 0.5f;

        for(int i = 0; i < grabLineCount; ++i) {
            float y = startY + i * grabLineSpacing;
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p0.x, y), ImVec2(p0.x + grabLineWidth, y + grabLineHeight), grabColor);
        }

        ImGui::Dummy(ImVec2(grabLineWidth + 4.0f, thumbnailSize.y));
        ImGui::EndGroup();
        ImGui::SameLine();

        ImGui::BeginGroup();

        float textureWidth = ((SDL_Texture*)(*it)->getTextureID())->w;
        float textureHeight = ((SDL_Texture*)(*it)->getTextureID())->h;

        ImVec2 uv0 = ImVec2((*it)->getX() / textureWidth, (*it)->getY() / textureHeight);
        ImVec2 uv1 = ImVec2(((*it)->getX() + (*it)->getWidth()) / textureWidth, ((*it)->getY() + (*it)->getHeight()) / textureHeight);

        if(ImGui::ImageButton("##thumbnailImg", (*it)->getTextureID(), thumbnailSize, uv0, uv1)) {
            _selectedFrame = *it;
        }

        if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {

        }

        ImGui::SameLine();
        ImGui::Text((*it)->getName().c_str());
        ImGui::EndGroup();
        ImGui::EndGroup();

        if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            ImGui::SetDragDropPayload("FRAME_PAYLOAD", &count, sizeof(int));
            ImGui::BeginGroup();
            ImGui::Image((*it)->getTextureID(), ImVec2(32, 32), uv0, uv1, ImVec4(1,1,1,0.5f), ImVec4(0, 0, 0, 0.0f));
            ImGui::SameLine();
            ImGui::Text((*it)->getName().c_str());
            ImGui::EndGroup();
            ImGui::EndDragDropSource();
        }

        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FRAME_PAYLOAD")) {
                IM_ASSERT(payload->DataSize == sizeof(int));
                int srcIndex = *(const int*)payload->Data;
                if (srcIndex != count) {
                    auto srcIt = _frames.begin();
                    std::advance(srcIt, srcIndex);

                    auto dstIt = _frames.begin();
                    std::advance(dstIt, count);

                    std::iter_swap(srcIt, dstIt);
                    _reproductionFrame = 0;
                }
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::PopID();
    }

    if(ImGui::Button("+", thumbnailSize)) {
        _openSpriteSelector = true;
    }
    ImGui::EndChild();

    if(_openSpriteSelector) {
        ImGui::SameLine();
        ImGui::BeginChild("##selectsprite", ImVec2(450, 400), true);
        drawSpriteSelector();
        ImGui::EndChild();
    }
}

void editor::render::modals::AnimationWizard::drawAnimation() {
    ImGui::BeginChild("##animationViewer", ImVec2(400, 530), true);
    if(!_frames.empty()) {
        if(_playing) _frameTimer += ImGui::GetIO().DeltaTime;

        if(_frameTimer >= _timeBetweenFrames) {
            _frameTimer = 0.0f;
            _reproductionFrame++;

            if(_reproductionFrame >= _frames.size()) {
                if(_loop) _reproductionFrame = 0;
                else {
                    _playing = false;
                    --_reproductionFrame;
                    _animationRunning = false;
                }
            }
        }

        auto it = _frames.begin();
        std::advance(it, _reproductionFrame);
        ImGui::Indent(20.0f);

        float textureWidth = ((SDL_Texture*)(*it)->getTextureID())->w;
        float textureHeight = ((SDL_Texture*)(*it)->getTextureID())->h;

        ImVec2 uv0 = ImVec2((*it)->getX() / textureWidth, (*it)->getY() / textureHeight);
        ImVec2 uv1 = ImVec2(((*it)->getX() + (*it)->getWidth()) / textureWidth, ((*it)->getY() + (*it)->getHeight()) / textureHeight);

        ImGui::Image((*it)->getTextureID(), ImVec2(350, 500), uv0, uv1);
        ImGui::Unindent(20.0f);
    }
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::drawSpriteSelector() {
    int count = 0;
    float thumbnailSize = 64.0f;
    float padding = 8.0f;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columns = (int)(panelWidth / (thumbnailSize + padding));
    if (columns < 1) columns = 1;

    for (auto it = _project->getSprites().begin(); it != _project->getSprites().end(); ++it) {
        ImGui::PushID(count);

        auto* sprite = it->second;

        float textureWidth = ((SDL_Texture*)sprite->getTextureID())->w;
        float textureHeight = ((SDL_Texture*)sprite->getTextureID())->h;

        ImVec2 uv0 = ImVec2(sprite->getX() / textureWidth, sprite->getY() / textureHeight);
        ImVec2 uv1 = ImVec2((sprite->getX() + sprite->getWidth()) / textureWidth, (sprite->getY() + sprite->getHeight()) / textureHeight);

        if (ImGui::ImageButton("spriteBtn", sprite->getTextureID(), ImVec2(thumbnailSize, thumbnailSize), uv0, uv1)) {
            _frames.push_back(sprite);
            _openSpriteSelector = false;
        }

        if ((count + 1) % columns != 0)
            ImGui::SameLine();

        ImGui::PopID();
        ++count;
    }
}

void editor::render::modals::AnimationWizard::drawAnimationControls() {
    ImGui::BeginChild("##animationReproductionControls", ImVec2(400, 100));
    {
        ImGui::Indent(15.0f);
        ImGui::Text(std::to_string(_reproductionFrame + 1).c_str());
        ImGui::SameLine();
        ImGui::PushFont(render::RenderManager::GetInstance().getFont("FA 900"));
        ImGui::BeginDisabled(_playing);
        if(ImGui::Button(ICON_FA_BACKWARD_STEP)) {
            if(!_playing) _reproductionFrame = 0;
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_BACKWARD)) {
            if(!_playing) {
                --_reproductionFrame;
                if(_reproductionFrame < 0) _reproductionFrame = _frames.size() - 1;
            }
        }
        ImGui::EndDisabled();

        bool colorPushed = false;

        ImGui::SameLine();
        if(_playing) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 1, 1));
            colorPushed = true;
        }
        if(ImGui::Button(ICON_FA_PLAY)) {
            if(!_animationRunning) _reproductionFrame = 0;
            _playing = true;
            _animationRunning = true;
        }
        if(_playing && colorPushed) {
            ImGui::PopStyleColor();
            colorPushed = false;
        }
        ImGui::SameLine();
        if(_animationRunning && !_playing) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 1, 1));
            colorPushed = true;
        }
        if(ImGui::Button(ICON_FA_PAUSE)) {
            _playing = false;
        }
        if(_animationRunning && !_playing && colorPushed) {
            ImGui::PopStyleColor();
            colorPushed = false;
        }
        ImGui::SameLine();
        if(!_animationRunning) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 1, 1));
            colorPushed = true;
        }
        if(ImGui::Button(ICON_FA_STOP)) {
            _playing = false;
            _reproductionFrame = 0;
            _animationRunning = false;
            _frameTimer = 0.0f;
        }
        if(!_animationRunning && colorPushed) {
            ImGui::PopStyleColor();
            colorPushed = false;
        }
        ImGui::SameLine();
        ImGui::BeginDisabled(_playing);
        if(ImGui::Button(ICON_FA_FORWARD)) {
            if(!_playing) {
                ++_reproductionFrame;
                if(_reproductionFrame >= _frames.size()) _reproductionFrame = 0;
            }
        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_FORWARD_STEP)) {
            if(!_playing)
                _reproductionFrame = _frames.size() - 1;
        }
        ImGui::EndDisabled();
        ImGui::PopFont();
        ImGui::Unindent(15.0f);
    }
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::setAnimationToModify(editor::resources::Animation *animation, bool modify) {
    _animationToModify = animation;
    _modify = modify;
}

std::vector<editor::resources::Sprite*> editor::render::modals::AnimationWizard::getSpritesVectorFromList() {
    std::vector<editor::resources::Sprite*> tmp;
    for(auto it = _frames.begin(); it != _frames.end(); ++it) {
        tmp.push_back((*it));
    }
    return tmp;
}

void editor::render::modals::AnimationWizard::getSpritesListFromVector(
        const std::vector<editor::resources::Sprite *> &frames) {
    for(int i = 0; i < frames.size(); ++i) {
        _frames.push_back(frames[i]);
    }
}



