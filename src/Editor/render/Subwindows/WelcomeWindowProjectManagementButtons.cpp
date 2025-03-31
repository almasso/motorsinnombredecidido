//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WelcomeWindowProjectManagementButtons.h"
#include "render/RenderManager.h"
#include "utils/IconsFontAwesome6.h"
#include "io/LocalizationManager.h"
#include "io/ProjectManager.h"
#include "render/WindowStack.h"
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include "render/Modals/CreateProjectModal.h"
#include "render/Modals/RenameProjectModal.h"
#include "render/Modals/DeleteProjectModal.h"

editor::render::subwindows::WelcomeWindowProjectManagementButtons::WelcomeWindowProjectManagementButtons(std::unordered_map<Project*, editor::render::modals::DeleteProjectModal*>& deleteProjects,
                                                                                                         std::unordered_map<Project*, editor::render::modals::RenameProjectModal*>& renameProjects,
                                                                                                         editor::render::modals::CreateProjectModal* createProject) :
Subwindow("WWPMB"), _deleteProjects(deleteProjects), _renameProjects(renameProjects), _createProject(createProject) {}

void editor::render::subwindows::WelcomeWindowProjectManagementButtons::beforeRender() {}

void editor::render::subwindows::WelcomeWindowProjectManagementButtons::onRender() {
    float leftPanelWidth = RenderManager::GetInstance().getWidth() / 2 - 90;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    float windowHeight = ImGui::GetContentRegionAvail().y;
    float rightPanelWidth = windowWidth - leftPanelWidth - 200;

    ImGui::SetCursorPosY(windowHeight / 2 - 150);

    ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
    if(ImGui::Button("##ButtonCreateProject", ImVec2(200, 150))) {
        _showCreateProject = true;
    }
    ImVec2 buttonPos = ImGui::GetItemRectMin();
    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 10, buttonPos.y - 20));
    ImGui::Text(ICON_FA_FOLDER_PLUS);
    ImGui::PopFont();

    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 60, buttonPos.y + 60));
    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 30"));
    ImGui::TextWrapped(io::LocalizationManager::GetInstance().getString("action.newproject").c_str());
    ImGui::PopFont();

    ImGui::NewLine();

    ImGui::PushFont(RenderManager::GetInstance().getFont("FA 900"));
    if(ImGui::Button("##ButtonOpenProject", ImVec2(200, 150))) {
        std::string route = searchProject();
        if(!route.empty()) io::ProjectManager::GetInstance().addProject(route);
    }
    buttonPos = ImGui::GetItemRectMin();
    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 10, buttonPos.y - 20));
    ImGui::Text(ICON_FA_FOLDER_OPEN);
    ImGui::PopFont();

    ImGui::SetCursorScreenPos(ImVec2(buttonPos.x + 60, buttonPos.y + 60));
    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 30"));
    ImGui::TextWrapped(io::LocalizationManager::GetInstance().getString("action.openproject").c_str());
    ImGui::PopFont();

    if(_showCreateProject) {
        _createProject->show();
        if(!_createProject->isOpen()) _showCreateProject = false;

    }

    editor::Project* createdProj = _createProject->getCreatedProject();
    if(createdProj != nullptr) {
        io::ProjectManager::GetInstance().addProject(createdProj);
        _renameProjects[createdProj] = new editor::render::modals::RenameProjectModal(createdProj);
        WindowStack::addWindowToStack(_renameProjects[createdProj]);
        _deleteProjects[createdProj] = new editor::render::modals::DeleteProjectModal(createdProj);
        WindowStack::addWindowToStack(_deleteProjects[createdProj]);
    }
}

std::string editor::render::subwindows::WelcomeWindowProjectManagementButtons::searchProject() {
    const char* fileExtension[] = {"*.lua"};
    const char* filePath = tinyfd_openFileDialog(
            io::LocalizationManager::GetInstance().getString("action.selectprojectfile").c_str(),
            "",
            1,
            fileExtension,
            io::LocalizationManager::GetInstance().getString("file.projectfile").c_str(),
            0);
    if(filePath == nullptr) return "";
    else return std::string(filePath);
}
