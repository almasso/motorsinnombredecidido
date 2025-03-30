//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "ModalWindow.h"
#include <imgui.h>

void editor::render::ModalWindow::render() {
    beforeRender();
    if(_wantsToShow) {
        ImGui::OpenPopup(_objectName.c_str());
        _wantsToShow = false;
        _isOpen = true;
    }
    if(ImGui::BeginPopupModal(_objectName.c_str(), NULL, _windowFlags)) {
        onRender();
        ImGui::EndPopup();
    }
}

void editor::render::ModalWindow::show() {
    _wantsToShow = true;
}
