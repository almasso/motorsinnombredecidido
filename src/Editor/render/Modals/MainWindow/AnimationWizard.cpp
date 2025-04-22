//
// Created by Alex Massó Martínez on 13/4/25.
//

#include "AnimationWizard.h"
#include "resources/Animation.h"
#include <SDl3/SDL.h>
#include "io/LocalizationManager.h"
#include "render/RenderManager.h"
#include "common/Project.h"
#include "utils/IconsFontAwesome6.h"

editor::render::modals::AnimationWizard::AnimationWizard(editor::Project *project) : ModalWindow("##animationWizard"), _project(project) {}

editor::render::modals::AnimationWizard::~AnimationWizard() {
    if(_frameTextures.size() != 0) {
        clearTextures();
    }
}

void editor::render::modals::AnimationWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::AnimationWizard::onRender() {
    drawControls();
    ImGui::SameLine();
    ImGui::BeginChild("##animationPart");
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
            copyFramesToList(_animationToModify->getFrames());
            framesToTextures();
            _timeBetweenFrames = _animationToModify->getTimeBetweenFrames();
        }
    }
    _isGivingName = true;

    ImGui::BeginChild("#animationControls", ImVec2(600, 720));
    {
        ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.animationwizard.animationame").c_str(),
                 _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

        if(_project->getTileset(_nameBuffer) != nullptr) _sameName = true;
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
        drawThumbnail();
    }
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::drawThumbnail() {
    ImGui::BeginChild("##animationFrameThumbnails", ImVec2(600, 400), true);
    {
        ImVec2 thumbailSize(64, 64);
        int count = 0;
        for(auto it = _frameTextures.begin(); it != _frameTextures.end(); ++it) {
            if(ImGui::ImageButton(std::string("##thumbnailImg" + count).c_str(), *it, thumbailSize)) {
                _selectedFrame = count;
            }

            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {

            }

            ImGui::SameLine();
            ImGui::Text(std::string(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.animationwizard.frame") + " " + std::to_string(count)).c_str());
            ++count;
        }
    }
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::drawAnimation() {

}

void editor::render::modals::AnimationWizard::drawAnimationControls() {
    ImGui::BeginChild("##animationReproductionControls");
    {
        ImGui::PushFont(render::RenderManager::GetInstance().getFont("FA 900"));
        if(ImGui::Button(ICON_FA_BACKWARD_STEP)) {

        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_BACKWARD)) {

        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_PLAY)) {

        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_PAUSE)) {

        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_STOP)) {

        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_FORWARD)) {

        }
        ImGui::SameLine();
        if(ImGui::Button(ICON_FA_FORWARD_STEP)) {

        }
        ImGui::PopFont();
    }
    ImGui::EndChild();
}

void editor::render::modals::AnimationWizard::setAnimationToModify(editor::resources::Animation *animation, bool modify) {
    _animationToModify = animation;
    _modify = modify;
}

void editor::render::modals::AnimationWizard::copyFramesToList(const std::vector<std::filesystem::path> &frames) {
    for(int i = 0; i < frames.size(); i++) {
        //_frames.push_back(frames[i]);
    }
}

void editor::render::modals::AnimationWizard::framesToTextures() {
    for(auto it = _frames.begin(); it != _frames.end(); ++it) {
        //_frameTextures.push_back(RenderManager::GetInstance().loadTexture(it->string()));
    }
}

void editor::render::modals::AnimationWizard::clearTextures() {
    for(auto it = _frameTextures.begin(); it != _frameTextures.end(); ++it) {
        RenderManager::GetInstance().destroyTexture(*it);
        *it = 0;
    }
    _frameTextures.clear();
}

void editor::render::modals::AnimationWizard::copyFramesToVector(std::vector<std::filesystem::path> &frames) {
    for(auto it = _frames.begin(); it != _frames.end(); ++it) {
        //frames.push_back(*it);
    }
}


