#ifndef SCENEBLUEPRINT_H
#define SCENEBLUEPRINT_H

#include <vector>
#include <Load/Resource.h>
#include "EntityBlueprint.h"

class SceneBlueprint final : public Resource {
private:
    std::vector<EntityBlueprint> _entities;

public:
    SceneBlueprint(std::string const &path);
    bool load() override;
    void unload() override;
    const std::vector<EntityBlueprint>& getEntities() const;
};



#endif //SCENEBLUEPRINT_H
