//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapWizard.h"
#include "io/LocalizationManager.h"
#include <imgui.h>
#include <imgui_internal.h>

editor::render::modals::MapWizard::MapWizard() : ModalWindow(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.title") + "") {
    _dimensions[0] = _dimensions[1] = 8;
}

editor::render::modals::MapWizard::~MapWizard() {}

void editor::render::modals::MapWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::MapWizard::onRender() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, io::LocalizationManager::GetInstance().getString("map.default").c_str(), sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
    }
    _isGivingName = true;

    ImGui::Spacing();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.mapname").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::Spacing();
    ImGui::InputInt2(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.dimensions").c_str(),
                      _dimensions);
    _dimensions[0] = ImClamp(_dimensions[0], 8, 512);
    _dimensions[1] = ImClamp(_dimensions[1], 8, 512);


    ImGui::Spacing();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("action.addmap").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isGivingName = false;
    }
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _isGivingName = false;
    }
}
