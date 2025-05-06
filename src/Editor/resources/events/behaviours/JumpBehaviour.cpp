//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "JumpBehaviour.h"

#include <imgui.h>
#include <io/LocalizationManager.h>
#include <resources/events/Event.h>

#define targetKey "target"

editor::resources::events::JumpBehaviour::JumpBehaviour(Event* event) :
    EventBehaviourTemplate(event),
    _target(-1){
}

editor::resources::events::JumpBehaviour::~JumpBehaviour() = default;

bool editor::resources::events::JumpBehaviour::read(sol::table const& params) {
    sol::optional<int> target = params.get<sol::optional<int>>(targetKey);
    if (!target.has_value())
        return false;
    _target = target.value();
    return true;
}

bool editor::resources::events::JumpBehaviour::writeParamsToEngine(std::ostream& behaviour, EventBuildDependencies& dependencies) {
    return true;
}


bool editor::resources::events::JumpBehaviour::render() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours.JumpBehaviour.target").c_str());
    return renderBehaviourSelector();
}


bool editor::resources::events::JumpBehaviour::writeParams(sol::table& params) {
    params[targetKey] = _target;
    return true;
}

bool editor::resources::events::JumpBehaviour::renderBehaviourSelector() {
    auto const& behaviours = _event->getBehaviours();
    std::string selectedBehaviour;
    if (_target < 0 || _target >= behaviours.size())
        selectedBehaviour = "invalid";
    else {
        auto selected = behaviours.begin();
        for (int i = 0; i < _target; ++i, ++selected);
        selectedBehaviour = (*selected)->getID();
    }

    std::string previewValue;
    if (_target >= 0) {
        previewValue += std::to_string(_target);
        previewValue += " - ";
    }
    previewValue += io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + selectedBehaviour);

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (!ImGui::BeginCombo((std::string("###jumpBehaviourSelector") + std::to_string(reinterpret_cast<long long>(this))).c_str(),
                           previewValue.c_str()))
        return false;

    bool edited = false;
    int i = 0;
    for (auto const& behaviour : behaviours) {
        bool isSelected = (i == _target);
        std::string label(std::to_string(i));
        label += " - ";
        label += io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours." + std::string(behaviour->getID()));
        if (ImGui::Selectable(label.c_str(), isSelected)) {
            if (!isSelected) {
                _target = i;
                edited = true;
            }
        }
        ++i;
    }
    ImGui::EndCombo();
    return edited;
}