#ifndef COMPONENTTEMPLATE_H
#define COMPONENTTEMPLATE_H

#include "Component.h"
#include <Utils/string_literal.h>

template<typename ComponentBase>
concept componentType  = std::is_base_of_v<Component, ComponentBase>;

template <string_literal componentName, componentType ComponentBase = Component>
class ComponentTemplate : public ComponentBase {
public:
    ComponentTemplate(ComponentData const*data) : ComponentBase(data) {
    }
    static inline int order = -1;
    static constexpr const char* id = componentName.value;
    int getOrder() const override { return order; }
};
#endif //COMPONENTTEMPLATE_H
