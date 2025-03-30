//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "RenameProjectModal.h"
#include <imgui.h>
#include "common/Project.h"
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"

editor::render::modals::RenameProjectModal::RenameProjectModal(editor::Project *project) : editor::render::ModalWindow(
        io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.renameproject.title") + " - " +
        project->getName() + "##" + project->getPath().string()), _project(project), _projectName(project->getName()),
        _projectRoute(project->getPath().string()) {}

void editor::render::modals::RenameProjectModal::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImGui::SetNextWindowSize(ImVec2(RenderManager::GetInstance().getWidth() / 2, 150));
}

void editor::render::modals::RenameProjectModal::onRender() {
    if(!_isRenaming) strncpy(_nameBuffer, _projectName.c_str(), sizeof(_nameBuffer) - 1);
    _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
    _isRenaming = true;

    ImGui::InputText(("##Renaming" + _projectRoute).c_str(), _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30);
    if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::Button(io::LocalizationManager::GetInstance().getString("action.renameproject").c_str(), ImVec2(120, 0))) {
        _project->updateName(std::string(_nameBuffer));
        _projectName = _project->getName();
        _objectName = io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.renameproject.title") + " - " +
                      _projectName + "##" + _projectRoute;
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isRenaming = false;
    }
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isRenaming = false;
    }
}