//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WINDOW_H
#define MOTORSINNOMBREDECIDIDO_WINDOW_H

#include <string>
#include <list>
#include <functional>

typedef int ImGuiWindowFlags;

namespace editor::render {
    class Window {
    public:
        template <std::convertible_to<std::string> T>
        explicit Window(T&& windowName) : _windowName(std::forward<T>(windowName)){}

        void render();

        [[nodiscard]] const std::string& getName() const;
    protected:
        std::string _windowName;

        ImGuiWindowFlags _windowFlags = 0;

        virtual void onRender() = 0;

        virtual void beforeRender() = 0;

    };
}


#endif //MOTORSINNOMBREDECIDIDO_WINDOW_H
