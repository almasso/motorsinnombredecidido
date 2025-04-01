#ifndef EVENTCONDITION_H
#define EVENTCONDITION_H

#include <sol/forward.hpp>

class Scene;
class Event;
class Entity;

class EventCondition {
protected:
    Scene* _scene;
    Entity* _entity;
    Event* _event;

public:
    EventCondition();
    virtual bool init(sol::table const& params) = 0;
    void setContext(Scene* scene, Entity* entity, Event* event);
    virtual void reset();
    virtual bool met() = 0;
    virtual ~EventCondition();

    static void RegisterToLua(sol::state& luaState);
};

#include <Utils/string_literal.h>

template<string_literal conditionName>
class EventConditionTemplate : public EventCondition {
public:
    static constexpr const char* id = conditionName.value;
};


#endif //EVENTCONDITION_H
