#ifndef EVENTCONDITIONFACTORY_H
#define EVENTCONDITIONFACTORY_H

#include <functional>
#include <unordered_map>
#include <string>
#include <sol/forward.hpp>

class Scene;
class Entity;
class Event;
class EventCondition;

class EventConditionFactory {
private:
    static std::unordered_map<std::string, std::function<EventCondition*()>> _factory;

    template <typename Condition>
    static void RegisterCondition() {
        _factory.insert({Condition::id, [](){
            return new Condition();
        }});
    }

    static void Init();
    static bool ReadCondition(sol::table const& condition, std::string& type, sol::table& params);
    static EventCondition* CreateCondition(std::string const& type, sol::table const& params, Scene* scene, Entity* entity, Event* event);

public:
    EventConditionFactory() = delete;
    static EventCondition* Create(sol::table const& condition, Scene* scene, Entity* entity, Event* event);

    static void RegisterToLua(sol::state& luaState);
};



#endif //EVENTCONDITIONFACTORY_H
