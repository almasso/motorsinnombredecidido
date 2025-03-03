//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_WINDOWSTACK_H
#define MOTORSINNOMBREDECIDIDO_WINDOWSTACK_H

#include <memory>
#include "utils/SearchableList.h"

namespace editor::render {

    class Window;

    /**
     * @~english
     * @brief Stack of active windows to render from bottom to top.
     *
     * @~spanish
     * @brief Pila de ventanas activas a renderizar de abajo a arriba.
     */
    class WindowStack {
    public:
        /**
         * @~english
         * @brief Adds a window to the stack.
         *
         * @param window Window to add.
         *
         * @~spanish
         * @brief Añade una ventana a la pila.
         *
         * @param window Ventana a añadir.
         */
        static void addWindowToStack(Window* window);

        /**
         * @~english
         * @brief Removes a window from the stack.
         *
         * @param window Window to remove.
         *
         * @~spanish
         * @brief Elimina una ventana de la pila.
         *
         * @param window Ventana a eliminar.
         */
        static void removeWindowFromStack(Window* window);

        /**
         * @~english
         * @brief Removes all windows from the stack.
         *
         * @~spanish
         * @brief Elimina todas las ventanas de la pila.
         */
        static void removeAllWindowsFromStack();

        /**
         * @~english
         * @brief Returns a window given its name.
         *
         * @param name Name of the window
         *
         * @return Window that has that name, or, \c nullptr in case there's no window in the stack.
         *
         * @~spanish
         * @brief Devuelve una ventana dado su nombre.
         *
         * @param name Nombre de la ventana
         *
         * @return Ventana que tiene ese nombre, o, \c nullptr en caso de que no esté en la pila.
         */
        static Window* getWindowByName(const std::string& name);

        /**
         * @~english
         * @brief Renders all windows in the stack from bottom (first one inserted) to top (last one inserted).
         *
         * @~spanish
         * @brief \c Renderiza todas las ventanas de la pila desde abajo (primera insertada) hasta arriba (última insertada).
         */
        static void renderWindows();
    private:
        /**
         * @~english
         * @brief SearchableList that will be used as a stack.
         *
         * @~spanish
         * @brief SearchableList que se va a usar como pila.
         */
        static SearchableList<Window*> _windowStack;

        /**
         * @~english
         * @brief Default constructor of WindowStack.
         *
         * @~spanish
         * @brief Constructora por defecto de WindowStack.
         */
        WindowStack() = default;

        /**
         * @~english
         * @brief Default destructor of WindowStack.
         *
         * @~spanish
         * @brief Destructora por defecto de WindowStack.
         */
        ~WindowStack() = default;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_WINDOWSTACK_H
