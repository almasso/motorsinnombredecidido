//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "Subwindow.h"

void editor::render::Subwindow::render() {
    beforeRender();
    ImGui::BeginChild(_objectName.c_str(), _size, _childFlags, _windowFlags);
    onRender();
    ImGui::EndChild();
}
