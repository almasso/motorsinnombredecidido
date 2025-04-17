//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "EventEditor.h"

#include "io/LocalizationManager.h"

editor::render::tabs::EventEditor::EventEditor(editor::Project* project) :
    WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.eventeditor") + ""),
    _project(project) {
}

editor::render::tabs::EventEditor::~EventEditor() {
}

void editor::render::tabs::EventEditor::save() {
}

void editor::render::tabs::EventEditor::beforeRender() {
    _itemFlags = 0;
}

void editor::render::tabs::EventEditor::onRender() {
}
