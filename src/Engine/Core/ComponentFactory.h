#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H
#include <unordered_map>
#include <functional>
#include "Component.h"
class ComponentFactory {
    private:
        std::unordered_map<std::string, std::function<Component*()>> _factory;
    public:
        ComponentFactory();
        Component* createComponent(const std::string& name, const ComponentData& data);
};


#endif //COMPONENTFACTORY_H
