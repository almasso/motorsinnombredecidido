//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_EDITORERROR_H
#define MOTORSINNOMBREDECIDIDO_EDITORERROR_H

#include <string>
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include <cstdlib>

#define editorAssert(condition, message) editor::EditorError::assert_impl(condition, message, __FILE__, __LINE__);
/**
 * @~english
 * @brief Class that will contain all Editor possible errors, and methods to show them to the programmer or user
 *
 * @~spanish
 * @brief Clase que contendrá todos los posibles errores del Editor, así como métodos para poder mostrarlos al programador o usuario
 */
namespace editor {
    class EditorError {
    public:
        template <std::convertible_to<std::string> T>
        static void assert_impl(bool condition, T&& message, const char* file, int line) {
#ifndef NDEBUG
            if(!condition) {
                std::string filename = file;
                std::string lineStr = std::to_string(line);
                std::string title = "Assert triggered";
                std::string msg = "Assert triggered at " + filename + ":" + lineStr + ":\n";
                msg += std::forward<std::string>(message);
                tinyfd_messageBox(
                        title.c_str(),
                        msg.c_str(),
                        "ok",
                        "error",
                        0
                        );
                std::exit(EXIT_FAILURE);
            }
#endif
        }

        template <std::convertible_to<std::string> T>
        static void showError(T&& message) {

        }
    };
}


#endif //MOTORSINNOMBREDECIDIDO_EDITORERROR_H
