#ifndef ENTITYBLUEPRINT_H
#define ENTITYBLUEPRINT_H

#include <vector>
#include <sol/sol.hpp>

class ComponentData;

class EntityBlueprint {
private:
    std::vector<EntityBlueprint> _children;
    std::vector<ComponentData*> _components;
    bool active;
    std::string handler;

    void readChildren(sol::table const& childrenBlueprints);
    void readComponents(sol::table const& componentBlueprints);

protected:
    void read(sol::table const& blueprint);
    void clear();

public:
    EntityBlueprint();
    explicit EntityBlueprint(sol::table const& blueprint);
    ~EntityBlueprint();
};



#endif //ENTITYBLUEPRINT_H
