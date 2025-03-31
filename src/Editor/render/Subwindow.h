//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_SUBWINDOW_H
#define MOTORSINNOMBREDECIDIDO_SUBWINDOW_H

#include "RenderObject.h"
#include <imgui.h>

namespace editor::render {
    class Subwindow : public RenderObject {
    public:
        template <std::convertible_to<std::string> T>
        explicit Subwindow(T&& windowName) : RenderObject(windowName) {}

        ~Subwindow() override = default;

        void render() final;

    protected:
        ImGuiWindowFlags _windowFlags = 0;

        ImGuiChildFlags _childFlags = 0;

        ImVec2 _size;

        virtual void beforeRender() = 0;

        virtual void onRender() = 0;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_SUBWINDOW_H
