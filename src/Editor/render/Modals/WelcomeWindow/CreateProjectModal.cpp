//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "CreateProjectModal.h"
#include "io/LocalizationManager.h"
#include "imgui.h"
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include "render/RenderManager.h"
#include "common/Project.h"
#include <imgui_internal.h>
#include "io/ProjectManager.h"

editor::render::modals::CreateProjectModal::CreateProjectModal() : ModalWindow(
        io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.createproject.title").c_str()) {}

editor::render::modals::CreateProjectModal::~CreateProjectModal() {
    _projectCreated = nullptr;
}

void editor::render::modals::CreateProjectModal::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::CreateProjectModal::onRender() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, io::LocalizationManager::GetInstance().getString("project.default").c_str(), sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
        strncpy(_routeBuffer, "", sizeof(_routeBuffer) - 1);
        _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
        strncpy(_fullRoute, "", sizeof(_fullRoute) - 1);
        _fullRoute[sizeof(_fullRoute) - 1] = '\0';
        _dimensions[0] = _dimensions[1] = 16;
    }
    _isGivingName = true;

    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.createproject.field.projectname").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::Spacing();
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("action.search").c_str())) {
        const char* route = tinyfd_selectFolderDialog(
                io::LocalizationManager::GetInstance().getString("action.selectprojectfolder").c_str(),
                "");
        if(route != nullptr) {
            std::string fR = std::string(route) + std::string(1, std::filesystem::path::preferred_separator);
            fR = std::filesystem::path(fR).lexically_normal().string();
            strncpy(_routeBuffer, fR.c_str(), sizeof(_routeBuffer) - 1);
            _routeBuffer[sizeof(_routeBuffer) - 1] = '\0';
        }
    }
    std::string route = std::string(_routeBuffer) + std::string(_nameBuffer);
    strncpy(_fullRoute, route.c_str(), sizeof(_fullRoute) - 1);
    _fullRoute[sizeof(_fullRoute) - 1] = '\0';

    ImGui::SameLine();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.createproject.field.projectroute").c_str(),
                     _fullRoute, IM_ARRAYSIZE(_fullRoute), ImGuiInputTextFlags_EnterReturnsTrue);

    if(io::ProjectManager::GetInstance().getProject(_fullRoute) != nullptr) _sameName = true;
    else _sameName = false;

    if(_sameName) {
        ImGui::TextColored(ImColor(255,0,0),io::LocalizationManager::GetInstance().getString("error.sameprojectroute").c_str());
    }

    ImGui::Spacing();
    ImGui::InputInt2(io::LocalizationManager::GetInstance().getString("window.welcomewindow.popup.createproject.field.tilesetdim").c_str(), _dimensions);
    _dimensions[0] = ImClamp(_dimensions[0], 16, 256);
    _dimensions[1] = ImClamp(_dimensions[1], 16, 256);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30);
    ImGui::BeginDisabled(std::string(_routeBuffer) == "" || _sameName);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("action.createproject").c_str(), ImVec2(120, 0))) {
        _projectCreated = new Project(std::string(_nameBuffer), std::string(_fullRoute), _dimensions);
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isGivingName = false;
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isGivingName = false;
        _projectCreated = nullptr;
    }
}

editor::Project *editor::render::modals::CreateProjectModal::getCreatedProject() {
    Project* pc = _projectCreated;
    _projectCreated = nullptr;
    return pc;
}
