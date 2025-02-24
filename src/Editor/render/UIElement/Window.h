//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WINDOW_H
#define MOTORSINNOMBREDECIDIDO_WINDOW_H

#include <string>
#include <list>

namespace editor::render::ui_element {

    class WindowComponent;

    class Window {
    public:
        Window(const std::string& windowName);

        virtual void render() = 0;

        void addComponent(WindowComponent* component);
    protected:
        std::string _windowName;

        std::list<WindowComponent*> _components;

    };
}


#endif //MOTORSINNOMBREDECIDIDO_WINDOW_H
