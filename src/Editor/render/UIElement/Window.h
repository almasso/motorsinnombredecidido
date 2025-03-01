//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WINDOW_H
#define MOTORSINNOMBREDECIDIDO_WINDOW_H

#include <string>
#include <list>

namespace editor::render::ui_element {

    class Window {
    public:
        Window(const std::string& windowName);

        virtual void render() = 0;

        void addWindow(Window* subwindow);

        const std::string& getName() const;
    protected:
        std::string _windowName;

        std::list<Window*> _subwindows;

    };
}


#endif //MOTORSINNOMBREDECIDIDO_WINDOW_H
