//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "DeleteProjectModal.h"
#include "common/Project.h"
#include "render/RenderManager.h"
#include "imgui.h"
#include "io/ProjectManager.h"
#include "io/LocalizationManager.h"

editor::render::modals::DeleteProjectModal::DeleteProjectModal(editor::Project *project) : editor::render::ModalWindow(
        io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.deleteproject.title") + " - " +
        project->getName() + "##" + project->getPath().string()), _project(project), _projectName(project->getName()),
        _projectRoute(project->getPath().string()) {}

void editor::render::modals::DeleteProjectModal::onRender() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.deleteproject.confirmation").c_str());
    ImGui::Separator();

    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.yes").c_str(), ImVec2(120, 0))) {
        _confirmedDeletion = true;
        ImGui::CloseCurrentPopup();
        _isOpen = false;
    }
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.no").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
    }
}

void editor::render::modals::DeleteProjectModal::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}
