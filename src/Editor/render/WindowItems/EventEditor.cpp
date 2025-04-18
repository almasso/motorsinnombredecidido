//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventEditor.h"

#include <render/RenderManager.h>
#include <render/WindowStack.h>

#include "common/Project.h"
#include "io/LocalizationManager.h"
#include <resources/events/Event.h>
#include <resources/events/EventBehaviour.h>
#include <resources/events/EventConditionFactory.h>

#include "render/Modals/MainWindow/EventWizard.h"


editor::render::tabs::EventEditor::EventEditor(editor::Project* project) :
    WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor") + ""),
    _project(project),
    _eventWizard(new modals::EventWizard(project)),
    _eventOpened(false),
    _selectedEvent(nullptr),
    _createdEvent(nullptr),
    _somethingModified(false) {
    WindowStack::addWindowToStack(_eventWizard);
}

editor::render::tabs::EventEditor::~EventEditor() {
    WindowStack::removeWindowFromStack(_eventWizard);
    delete _eventWizard;
    _eventWizard = nullptr;
}

void editor::render::tabs::EventEditor::save() {
    _somethingModified = false;
    _project->saveEverything();
}

void editor::render::tabs::EventEditor::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::EventEditor::onRender() {

    ImGui::BeginChild("##event", ImVec2(RenderManager::GetInstance().getWidth()/5, 0));

    ImGui::BeginChild("##eventSelector", ImVec2(0, 75), true);
    renderEventDropDown();
    handleEventWizard();
    ImGui::EndChild();

    ImGui::BeginChild("##conditionEditor", ImVec2(0, 0), true);
    renderConditionEditor();
    ImGui::EndChild();

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("##behaviourEditor", ImVec2(0, 0), true);
    renderBehaviourEditor();
    ImGui::EndChild();

    if(_somethingModified && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        save();
    }
}

void editor::render::tabs::EventEditor::renderEventDropDown() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.event").c_str());
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth() / 5 - 20);
    if (!ImGui::BeginCombo("##eventDropdown", _selectedEvent != nullptr ? _selectedEvent->getName().c_str() : io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.eventselector").c_str()))
        return;

    auto const& events = _project->getEvents();
    for(auto it = events.begin(); it != events.end();) {
        bool isSelected = _selectedEvent == it->second;
        if (renderSelectableEvent(it))
            ++it;
        else if (isSelected)
            _selectedEvent = nullptr;
    }
    ImGui::Separator();
    if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.createevent").c_str())) {
        _createdEvent = new editor::resources::events::Event();
        _eventWizard->setEventToModify(_createdEvent);
        _eventWizard->show();
    }

    ImGui::EndCombo();
}

bool editor::render::tabs::EventEditor::renderSelectableEvent(std::unordered_map<std::string, editor::resources::events::Event*>::const_iterator& it) {
    auto [eventName, event] = *it;
    if(ImGui::Selectable(eventName.c_str(), event == _selectedEvent)) {
        _selectedEvent = event;
        if(ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup((eventName + "eventoptions").c_str());
        }
    }

    if(ImGui::BeginPopupContextItem((eventName + "eventoptions").c_str())) {
        bool stillValid = renderSelectableEventOptions(it);
        ImGui::EndPopup();
        return stillValid;
    }
    return true;
}

bool editor::render::tabs::EventEditor::renderSelectableEventOptions(std::unordered_map<std::string, resources::events::Event*>::const_iterator& it) {
    auto [eventName, event] = *it;
    if(ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.edit").c_str())) {
        _eventWizard->setEventToModify(event, true);
        _eventWizard->show();
        ImGui::CloseCurrentPopup();
    }
    bool stillValid = true;
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::MenuItem(io::LocalizationManager::GetInstance().getString("action.delete").c_str())) {
        it = _project->removeEvent(eventName);
        delete event;
        event = nullptr;
        _somethingModified = true;
        stillValid = false;
        ImGui::CloseCurrentPopup();
    }
    ImGui::PopStyleColor();
    return stillValid;
}

void editor::render::tabs::EventEditor::handleEventWizard() {
    if (_eventOpened && !_eventWizard->isOpen()) {
        if (_createdEvent != nullptr) {
            if (_createdEvent->isInitialized()) {
                _project->addEvent(_createdEvent);
                _createdEvent = nullptr;
                _somethingModified = true;
            }
            else {
                delete _createdEvent;
                _createdEvent = nullptr;
            }
            _eventOpened = false;
        }
        else {
            _eventOpened = false;
            _project->refreshEvents();
            _somethingModified = true;
        }
    }

    if(_eventWizard->hasBeenCalled())
        _eventOpened = true;
}

void editor::render::tabs::EventEditor::renderConditionEditor() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition").c_str());
    if (_selectedEvent == nullptr)
        return;

    renderConditionSelector();
    if (_selectedEvent->getCondition()->render())
        _somethingModified = true;
}

void editor::render::tabs::EventEditor::renderBehaviourEditor() {
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.behaviours").c_str());
    if (_selectedEvent == nullptr)
        return;
    auto& behaviours = _selectedEvent->getBehaviours();
    for (auto& behaviour : behaviours) {
        behaviour->render();
    }
}

void editor::render::tabs::EventEditor::renderConditionSelector() {
    std::string selectedCondition(_selectedEvent->getCondition()->getID());

    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth() / 5 - 20);
    if (!ImGui::BeginCombo( "##editorConditionSelector",
        io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + selectedCondition).c_str()))
        return;

    auto const& conditions = resources::events::EventConditionFactory::GetKeys();
    for (auto const& condition : conditions) {
        bool isSelected = (condition == selectedCondition);
        if (ImGui::Selectable(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor.condition." + condition).c_str(), isSelected)) {
            if (!isSelected) {
                _selectedEvent->setCondition(condition);
                _somethingModified = true;
            }
        }
    }

    ImGui::EndCombo();
}