#ifndef MAPCOMPONENT_H
#define MAPCOMPONENT_H
#include <Core/ComponentTemplate.h>
#include <unordered_set>
#include <string>
#include <Collisions/Collider.h>

class OverworldManager;

class MapComponent : public ComponentTemplate<"MapComponent"> {
    private:
        std::unordered_set<std::string> _adjacentMaps;
        OverworldManager* _manager;
        Collider* _collider;
        Collider* _player;
    public:
        MapComponent(const ComponentData* data);
        bool init() override;
        bool update() override;
        const std::unordered_set<std::string>& getAdjacentMaps() const;
};

#endif //MAPCOMPONENT_H
