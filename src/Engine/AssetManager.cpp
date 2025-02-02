#include "AssetManager.h"
#include "RPGError.h"
#include <SDL3/SDL.h>

bool AssetManager::readFile(const std::string& filename, char*& fileData, int& fileSize) {
    SDL_IOStream* file = SDL_IOFromFile(filename.c_str(), "r");
    if (!file) {
        RPGError::ShowError("Error al abrir el archivo", "Error al abrir el archivo: " + filename + " - " + SDL_GetError());
        return false;
    }

    fileSize = static_cast<int>(SDL_GetIOSize(file));
    if (fileSize <= 0) {
        RPGError::ShowError("Error al obtener el tamaño del archivo", "Error al obtener el tamaño del archivo: " + filename + " - " + SDL_GetError());
        SDL_CloseIO(file);
        return false;
    }

    fileData = new char[fileSize + 1];  // +1 para el null terminator
    if (SDL_ReadIO(file, fileData, fileSize) != fileSize) {
        RPGError::ShowError("Error al leer el archivo", "Error al leer el archivo: " + filename + " - " + SDL_GetError());
        delete[] fileData;
        SDL_CloseIO(file);
        return false;
    }
    fileData[fileSize] = '\0';

    SDL_CloseIO(file);
    return true;
}
