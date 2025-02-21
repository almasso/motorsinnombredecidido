#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>

class AssetManager {
public:
    static bool readFile(const std::string& filename, char*& fileData, int& fileSize);
};


#endif //ASSETMANAGER_H
