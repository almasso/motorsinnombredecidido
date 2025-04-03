//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "WindowItem.h"

void editor::render::WindowItem::render() {
    beforeRender();
    ImGui::BeginTabItem(_objectName.c_str(), nullptr, _itemFlags);
    onRender();
    ImGui::EndTabItem();
}