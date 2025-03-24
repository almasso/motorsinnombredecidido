#ifndef ENTITYBLUEPRINT_H
#define ENTITYBLUEPRINT_H

#include <vector>
#include <sol/sol.hpp>

class ComponentData;

class EntityBlueprint {
private:
    std::vector<EntityBlueprint> _children;
    std::vector<ComponentData*> _components;
    bool _active;
    std::string _handler;

    void readChildren(sol::table const& childrenBlueprints);
    void readComponents(sol::table const& componentBlueprints);

protected:
    void read(sol::table const& blueprint);
    void clear();

public:
    EntityBlueprint();
    explicit EntityBlueprint(sol::table const& blueprint);
    ~EntityBlueprint();
    const std::vector<EntityBlueprint>& getChildren() const;
    const std::vector<ComponentData*>& getComponents() const;
    bool getActive() const;
    const std::string& getHandler() const;


};



#endif //ENTITYBLUEPRINT_H
