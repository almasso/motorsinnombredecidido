//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_EDITORRESOURCE_H
#define MOTORSINNOMBREDECIDIDO_EDITORRESOURCE_H

#include <string>

namespace editor::resources {
    class EditorResource {
    public:
        template <std::convertible_to<std::string> T>
        EditorResource(T&& id) : _id(std::forward<T>(id)) {}

        virtual bool readFromLua(std::string const& name) = 0;

        virtual void writeToLua() = 0;

        virtual void writeToEngineLua() = 0;
    protected:
        std::string _id;
    };
}

#endif //MOTORSINNOMBREDECIDIDO_EDITORRESOURCE_H
