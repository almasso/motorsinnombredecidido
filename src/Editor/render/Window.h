//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_WINDOW_H
#define RPGBAKER_WINDOW_H

#include "RenderObject.h"

typedef int ImGuiWindowFlags;

namespace editor::render {
    class Window : public RenderObject {
    public:
        template <std::convertible_to<std::string> T>
        explicit Window(T&& windowName) : RenderObject(windowName) {}

        ~Window() override = default;

        void render() final;
    protected:
        ImGuiWindowFlags _windowFlags = 0;

        virtual void beforeRender() = 0;

        virtual void onRender() = 0;

    };
}


#endif //RPGBAKER_WINDOW_H
