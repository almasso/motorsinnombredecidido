#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <sol/forward.hpp>

class Game;
class Scene;
class Entity;
class EventCondition;
class EventBehaviour;

class Event {
private:
    Game* _game;
    Scene* _scene;
    Entity* _entity;
    EventCondition* _condition;
    std::vector<EventBehaviour*> _behaviours;

    std::vector<EventBehaviour*>::iterator _currentBehaviour;
    bool _isPaused;

    bool initCondition(sol::table const& event);
    bool insertBehaviour(sol::table const& behaviour);
    bool initBehaviours(sol::table const& event);

    Event(Event const& other);
    explicit Event(Game* game, Scene* scene, Entity* entity);
    bool init(sol::table const& event);

public:
    static Event* Create(Game* game, Scene* scene, Entity* entity, sol::table const& event);
    ~Event();

    void start();
    void resume();
    void pause();
    void stop();

    void update();
};



#endif //EVENT_H
