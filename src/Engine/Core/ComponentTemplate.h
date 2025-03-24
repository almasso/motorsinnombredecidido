#ifndef COMPONENTTEMPLATE_H
#define COMPONENTTEMPLATE_H
#include <algorithm>
#include "Component.h"

template <auto N>
struct string_literal {
    constexpr string_literal(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    char value[N];
};

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
