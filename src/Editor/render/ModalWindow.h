//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MODALWINDOW_H
#define MOTORSINNOMBREDECIDIDO_MODALWINDOW_H

#include "RenderObject.h"

typedef int ImGuiWindowFlags;

namespace editor::render {
    class ModalWindow : public RenderObject {
    public:
        template <std::convertible_to<std::string> T>
        explicit ModalWindow(T&& windowName) : RenderObject(windowName) {}

        virtual void show();

        bool isOpen() const {return _isOpen;}

        bool hasBeenCalled() const {return _wantsToShow;}

        void render() final;
    protected:
        ImGuiWindowFlags _windowFlags = 0;

        bool _isOpen = false;

        bool _wantsToShow = false;

        virtual void onRender() = 0;

        virtual void beforeRender() = 0;
    };
}

#endif //MOTORSINNOMBREDECIDIDO_MODALWINDOW_H
