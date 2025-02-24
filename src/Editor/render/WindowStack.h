//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WINDOWSTACK_H
#define MOTORSINNOMBREDECIDIDO_WINDOWSTACK_H

#include <memory>
#include "utils/IteratableStack.h"

namespace editor::render {
    namespace ui_element {
        class Window;
    }

    class WindowStack {
    public:
        static void addWindowToStack(ui_element::Window* window);
        //static void removeWindowFromStack(ui)
    private:
        static IteratableStack<ui_element::Window*> _windowStack;
        WindowStack() = default;
        ~WindowStack() = default;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_WINDOWSTACK_H
