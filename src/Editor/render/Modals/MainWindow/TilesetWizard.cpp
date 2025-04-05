//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "TilesetWizard.h"
#include "io/LocalizationManager.h"
#include <imgui.h>
#include "utils/tinyfiledialogs/tinyfiledialogs.h"

editor::render::modals::TilesetWizard::TilesetWizard() : ModalWindow(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.title") + "") {}

void editor::render::modals::TilesetWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::TilesetWizard::onRender() {
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

    ImGui::Spacing();
    ImGui::InputInt2(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.tiledim").c_str(), _dimensions);

    ImGui::Spacing();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.tilesetwizard.tilename").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("action.addtileset").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
    }
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
    }
}
