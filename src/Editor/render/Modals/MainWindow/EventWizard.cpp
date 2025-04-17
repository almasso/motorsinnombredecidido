//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventWizard.h"

#include <io/LocalizationManager.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <common/Project.h>
#include <resources/events/Event.h>
#include <resources/events/EventConditionFactory.h>


editor::render::modals::EventWizard::EventWizard(Project* project) :
    ModalWindow(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.eventwizard.title") + ""),
    _project(project),
    _eventToModify(nullptr),
    _modify(false),
    _isGivingName(false),
    _sameName(false),
    _nameBuffer{},
    _condition("OnStart"){
}

editor::render::modals::EventWizard::~EventWizard() {
}

void editor::render::modals::EventWizard::setEventToModify(editor::resources::events::Event* event, bool modify) {
    _eventToModify = event;
    _modify = modify;
}

void editor::render::modals::EventWizard::beforeRender() {
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
    _windowFlags |= flags;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void editor::render::modals::EventWizard::onRender() {
    renderNameSelector();
    renderConditionSelector();
    ImGui::Spacing();
    renderAddCancelButtons();
}

void editor::render::modals::EventWizard::renderNameSelector() {
    if(!_isGivingName) {
        strncpy(_nameBuffer, !_eventToModify->getName().empty() ?
                                 _eventToModify->getName().c_str() :
                                 io::LocalizationManager::GetInstance().getString("event.default").c_str(), sizeof(_nameBuffer) - 1);
        _nameBuffer[sizeof(_nameBuffer) - 1] = '\0';
    }
    _isGivingName = true;

    ImGui::Spacing();
    ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.popup.eventwizard.eventname").c_str(),
                     _nameBuffer, IM_ARRAYSIZE(_nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

    if(_project->getEvent(_nameBuffer) != nullptr)
        _sameName = true;
    else _sameName = false;

    if(!_eventToModify->getName().empty() && _eventToModify->getName() == _nameBuffer)
        _sameName = false;

    if(_sameName) {
        ImGui::TextColored(ImColor(255,0,0),io::LocalizationManager::GetInstance().getString("error.sameeventname").c_str());
    }
}

void editor::render::modals::EventWizard::renderConditionSelector() {
    if (!ImGui::BeginCombo(
        io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition").c_str(),
        io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + _condition).c_str()))
        return;

    auto const& conditions = resources::events::EventConditionFactory::GetKeys();
    for (auto const& condition : conditions) {
        bool isSelected = (condition == _condition);
        if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + condition).c_str(), isSelected)) {
            _condition = condition;
        }
    }

    ImGui::EndCombo();
}


void editor::render::modals::EventWizard::renderAddCancelButtons() {
    ImGui::BeginDisabled(_sameName);
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString(_modify ? "action.edit" : "action.add").c_str(), ImVec2(120, 0))) {
        _eventToModify->init(_nameBuffer, _condition);
        ImGui::CloseCurrentPopup();
        _eventToModify = nullptr;
        _isOpen = false;
        _isGivingName = false;
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button(io::LocalizationManager::GetInstance().getString("window.global.cancel").c_str(), ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        _isOpen = false;
        _eventToModify = nullptr;
        _isGivingName = false;
    }
}