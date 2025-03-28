#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <unordered_map>
#include <string>
#include <Core/ComponentTemplate.h>
#include <sol/forward.hpp>

class Event;

class EventHandler : public ComponentTemplate<"EventHandler"> {
private:
    std::unordered_map<std::string, Event*> _events;

    bool addEvent(std::string const& name, sol::table const& eventTable);
public:
    explicit EventHandler(ComponentData const* data);
    ~EventHandler() override;
    bool init() override;
    bool update() override;
};


#endif //EVENTHANDLER_H
