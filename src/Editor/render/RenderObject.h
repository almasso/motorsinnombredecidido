//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_RENDEROBJECT_H
#define RPGBAKER_RENDEROBJECT_H

#include <string>

namespace editor::render {
    class RenderObject {
    public:
        template <std::convertible_to<std::string> T>
        explicit RenderObject(T&& name) : _objectName(std::forward<std::string>(name)) {}

        virtual ~RenderObject() = default;

        virtual void render() = 0;

        [[nodiscard]] const std::string& getName() const {return _objectName;}
    protected:
        std::string _objectName;
    };
}

#endif //RPGBAKER_RENDEROBJECT_H
