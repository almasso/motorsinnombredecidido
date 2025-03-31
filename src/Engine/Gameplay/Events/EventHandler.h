#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <unordered_map>
#include <string>
#include <sol/forward.hpp>
#include <Core/ComponentTemplate.h>

class Event;

class EventHandler : public ComponentTemplate<EventHandler, "EventHandler"> {
private:
    std::unordered_map<std::string, Event*> _events;

    bool addEvent(std::string const& name, sol::table const& eventTable);
public:
    explicit EventHandler(ComponentData const* data);
    ~EventHandler() override;
    bool init() override;
    bool update() override;

    Event* getEvent(std::string const& name);

    static void RegisterToLua(sol::state& luaState);
};


#endif //EVENTHANDLER_H
