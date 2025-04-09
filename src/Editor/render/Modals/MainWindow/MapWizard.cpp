//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "MapWizard.h"
#include "io/LocalizationManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "common/Project.h"
#include "resources/Map.h"

editor::render::modals::MapWizard::MapWizard(editor::Project* project) :
    ModalWindow(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.title") + ""),
    _project(project) {}

editor::render::modals::MapWizard::~MapWizard() = default;

void editor::render::modals::MapWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::MapWizard::onRender() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, _mapToModify->getName() != "" ? _mapToModify->getName().c_str() :
        io::LocalizationManager::GetInstance().getString("map.default").c_str(), sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
        _dimensions[0] = _mapToModify->getMapWidth();
        _dimensions[1] = _mapToModify->getMapHeight();
        _layers = _mapToModify->getLayers();
    }
    _isGivingName = true;

    ImGui::Spacing();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.mapname").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    if(_project->getMap(_nameBuffer) != nullptr && !_modify) _sameName = true;
    else _sameName = false;

    if(_sameName) {
        ImGui::TextColored(ImColor(255,0,0),io::LocalizationManager::GetInstance().getString("error.samemapname").c_str());
    }

    ImGui::Spacing();
    ImGui::InputInt2(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.dimensions").c_str(),
                      _dimensions);
    _dimensions[0] = ImClamp(_dimensions[0], 8, 512);
    _dimensions[1] = ImClamp(_dimensions[1], 8, 512);

    ImGui::InputInt(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.mapwizard.layers").c_str(),
                     &_layers);
    _layers = ImClamp(_layers, 1, 16);


    ImGui::Spacing();
    ImGui::BeginDisabled(_sameName);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("action.addmap").c_str(), ImVec2(120, 0))) {
        _mapToModify->init(_nameBuffer, _dimensions[0], _dimensions[1], _layers);
        ImGui::CloseCurrentPopup();
        _mapToModify = nullptr;
        _isOpen = false;
        _isGivingName = false;
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _mapToModify = nullptr;
        _isGivingName = false;
    }
}

void editor::render::modals::MapWizard::setMapToModify(editor::resources::Map *map, bool modify) {
    _modify = modify;
    _mapToModify = map;
}
