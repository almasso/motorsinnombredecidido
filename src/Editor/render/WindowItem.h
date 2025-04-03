//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WINDOWITEM_H
#define MOTORSINNOMBREDECIDIDO_WINDOWITEM_H

#include "RenderObject.h"
#include <imgui.h>

namespace editor::render {
    class WindowItem : public RenderObject {
    public:
        template <std::convertible_to<std::string> T>
        WindowItem(T&& name) : RenderObject(name) {}

        ~WindowItem() override = default;

        void render() final;

    protected:
        ImGuiTabItemFlags _itemFlags = 0;

        virtual void beforeRender() = 0;

        virtual void onRender() = 0;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_WINDOWITEM_H
