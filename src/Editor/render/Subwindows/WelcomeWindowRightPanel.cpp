//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WelcomeWindowRightPanel.h"
#include "render/RenderManager.h"
#include "io/LocalizationManager.h"
#include "common/Project.h"
#include "io/ProjectManager.h"
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include "render/WindowStack.h"
#include "render/Modals/RenameProjectModal.h"
#include "render/Modals/DeleteProjectModal.h"
#include "render/Modals/CreateProjectModal.h"

editor::render::subwindows::WelcomeWindowRightPanel::WelcomeWindowRightPanel(std::unordered_map<Project*, editor::render::modals::DeleteProjectModal*>& deleteProjects,
                                                                             std::unordered_map<Project*, editor::render::modals::RenameProjectModal*>& renameProjects)
                                                                             : Subwindow("WWRP"), _deleteProjects(deleteProjects),
                                                                             _renameProjects(renameProjects) {}

void editor::render::subwindows::WelcomeWindowRightPanel::beforeRender() {
    float leftPanelWidth = RenderManager::GetInstance().getWidth() / 2 - 90;
    float windowWidth = ImGui::GetContentRegionAvail().x;
    float windowHeight = ImGui::GetContentRegionAvail().y;
    float rightPanelWidth = windowWidth - leftPanelWidth - 200;
    _size = ImVec2(rightPanelWidth - 160, windowHeight);
    _childFlags = 1;
}

void editor::render::subwindows::WelcomeWindowRightPanel::onRender() {
    ImGui::PushItemWidth(300);
    // Barra de búsqueda
    ImGuiTextFilter filter;

    char *buf = filter.InputBuf;
    if (ImGui::InputTextWithHint("##filterProjects", io::LocalizationManager::GetInstance().getString(
            "window.welcomewindow.searchbar").c_str(), buf, IM_ARRAYSIZE(filter.InputBuf))) {
        filter.Build();
    }

    ImGui::SameLine();
    if (ImGui::Button("X")) {
        filter.Clear();
    }

    ImGui::PopItemWidth();

    ImGui::Spacing();

    // Lista de botones con los proyectos
    ImGui::BeginChild("PL", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
    {
        for (Project *pr: io::ProjectManager::GetInstance().getProjects()) {
            if(!pr->isSetToDelete()) {
                if (filter.PassFilter(pr->getName().c_str()) || filter.PassFilter(pr->getPath().string().c_str())) {
                    if (pr != *io::ProjectManager::GetInstance().getProjects().begin()) {
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                    drawProjectButton(pr);
                }
            }
        }
    }
    ImGui::EndChild();
}

void editor::render::subwindows::WelcomeWindowRightPanel::drawProjectButton(editor::Project *project) {
    const std::string& projectRoute = project->getPath().string();
    const std::string& projectName = project->getName();
    const std::tm& lastModified = project->getLastModificationTime();

    ImVec2 buttonSize = ImVec2(-1, 120);

    ImGui::BeginDisabled(!project->isFound());
    if(ImGui::Button(std::string("##But" + projectRoute).c_str() , buttonSize)) {
        // Abrir la ventana principal del editor
    }
    ImGui::EndDisabled();

    if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && !project->isFound()) {
        ImGui::SetTooltip(io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.projectnotfound").c_str());
    }

    if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup((projectRoute + "Options").c_str());
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - buttonSize.y);

    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 40"));
    ImGui::Text(projectName.c_str());
    ImGui::PopFont();

    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 20"));
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), projectRoute.c_str());
    ImGui::PopFont();

    std::ostringstream oss;
    oss << std::put_time(&lastModified, "%d/%m/%Y %H:%M:%S");
    ImGui::PushFont(RenderManager::GetInstance().getFont("Raleway 16"));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - buttonSize.y + 145);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), std::string(io::LocalizationManager::GetInstance().getString("window.welcomewindow.lastmodified") + " "
                                                                   + oss.str()).c_str());
    ImGui::PopFont();

    if (ImGui::BeginPopupContextItem((projectRoute + "Options").c_str())) {
        ImGui::BeginDisabled(project->isFound());
        if(ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.relocateproject").c_str())) {
            std::string route = searchProject();
            std::filesystem::path p(route);
            if(!route.empty()) project->updatePath(p.parent_path().string());
        }
        ImGui::EndDisabled();
        ImGui::BeginDisabled(!project->isFound());
        if (ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.renameproject").c_str())) {
            _showRenameProject = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndDisabled();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        if (ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.deleteproject").c_str())) {
            _showDeleteConfirmation = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }

    if(_showDeleteConfirmation) {
        _deleteProjects[project]->show();
        if(_deleteProjects[project]->hasConfirmedDeletion()) {
            delete _deleteProjects[project];
            _deleteProjects.erase(project);
            delete _renameProjects[project];
            _renameProjects.erase(project);
        }
        if(!_deleteProjects[project]->isOpen()) _showDeleteConfirmation = false;
    }
    if(_showRenameProject) {
        _renameProjects[project]->show();
        if(!_renameProjects[project]->isOpen()) _showRenameProject = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

std::string editor::render::subwindows::WelcomeWindowRightPanel::searchProject() {
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
