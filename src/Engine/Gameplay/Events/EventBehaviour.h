#ifndef EVENTBEHAVIOUR_H
#define EVENTBEHAVIOUR_H

#include <sol/table.hpp>
#include <sol/function.hpp>

class Game;
class Scene;
class Entity;
class Event;

class EventBehaviour {
private:
    Game* _game;
    Scene* _scene;
    Entity* _entity;
    Event* _event;

    sol::table _self;
    sol::function _act;
    sol::function _done;
    sol::function _ended;
    sol::function _updateEnd;

    EventBehaviour(Game* game, Scene* scene, Entity* entity, Event* event);
    bool init(sol::table const& behaviour);
public:
    static EventBehaviour* Create(Game* game, Scene* scene, Entity* entity, Event* event, sol::table const& behaviour);
    bool act();
    bool done() const;
    bool ended() const;
    void updateEnd();
};



#endif //EVENTBEHAVIOUR_H
