#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H
#include <unordered_map>
#include <functional>
#include <string>

class Component;
class ComponentFactory {
    private:
    std::unordered_map<std::string, std::function<Component*()>> _factory;
    int _numComponents;
    public:
    ComponentFactory();
    Component* createComponent(const std::string &name);
    int getOrder(const std::string &name);
    template<typename ComponentType>
    void registerComponent() {
        if (_factory.insert({ComponentType::id, [] () -> Component* {
            return new ComponentType();
        }}).second) {
            ComponentType::order = _numComponents++;
        }
    }
};


#endif //COMPONENTFACTORY_H
