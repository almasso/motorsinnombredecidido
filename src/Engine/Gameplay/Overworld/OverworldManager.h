#ifndef OVERWORLDMANAGER_H
#define OVERWORLDMANAGER_H
#include <Core/ComponentTemplate.h>
#include <unordered_map>
#include <unordered_set>
#include <string>

class Entity;
class MapComponent;

class OverworldManager : public ComponentTemplate<"OverworldManager"> {
	private:
		std::unordered_map<std::string, Entity*> _loadedMaps;
		std::unordered_map<std::string, Entity*> _unloadedMaps;
		Entity* addMap(const std::string& mapName);
    public:
    	OverworldManager(const ComponentData* data);
		bool init() override;
		bool changeMap(std::unordered_set<std::string> adjacent);
};

#endif //OVERWORLDMANAGER_H
