#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H
#include <unordered_map>
#include <functional>
#include <string>

class Component;
class ComponentData;
class ComponentFactory {
    private:
    std::unordered_map<std::string, std::function<Component*(ComponentData const*)>> _factory;
    int _numComponents;
    public:
    ComponentFactory();
    Component* createComponent(ComponentData const* data);
    int getOrder(const std::string &name);
    template<typename ComponentType>
    void registerComponent() {
        if (_factory.insert({ComponentType::id, [] (ComponentData const* data) -> Component* {
            return new ComponentType(data);
        }}).second) {
            ComponentType::order = _numComponents++;
        }
    }
};


#endif //COMPONENTFACTORY_H
