//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_EDITORERROR_H
#define RPGBAKER_EDITORERROR_H

#include <string>
#include "utils/tinyfiledialogs/tinyfiledialogs.h"
#include <cstdlib>

#define editorAssert(condition, message) editor::EditorError::assert_impl(condition, message, __FILE__, __LINE__);
#define showError(message) editor::EditorError::showError_impl(message, __FILE__, __LINE__);
#define showWarning(message) editor::EditorError::showWarning_impl(message, __FILE__, __LINE__);
#define showUserWarning(message) editor::EditorError::showWarning_impl(message, nullptr, -1);
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
                msg += std::forward<T>(message);
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
        static void showError_impl(T&& message, const char* file, int line) {
            std::string filename = file;
            std::string lineStr = std::to_string(line);
            std::string title = "Error detected";
            std::string msg = "Error detected at " + filename + ":" + lineStr + ":\n";
            msg += std::forward<T>(message);
            tinyfd_messageBox(
                    title.c_str(),
                    msg.c_str(),
                    "ok",
                    "error",
                    0
                    );
        }

        template <std::convertible_to<std::string> T>
        static void showWarning_impl(T&& message, const char* file, int line) {
            std::string filename, lineStr, msg;
            if(file != nullptr) filename = file;
            if(line != -1) lineStr = std::to_string(line);
            std::string title = "Warning!";
            if(file != nullptr && line != -1) msg = "Warning at " + filename + ":" + lineStr + ":\n";
            else msg = "Warning! ";
            msg += std::forward<T>(message);
            tinyfd_messageBox(
                    title.c_str(),
                    msg.c_str(),
                    "ok",
                    "warning",
                    0
            );
        }
    };
}


#endif //RPGBAKER_EDITORERROR_H
