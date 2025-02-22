//
// Created by Usuario on 01/02/2025.
//

#include "RPGError.h"
#include <SDL3/SDL_messagebox.h>

void RPGError::ShowError(const std::string& title, const std::string& body) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(),body.c_str(), nullptr);
}
