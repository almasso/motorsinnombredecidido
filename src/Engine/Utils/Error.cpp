#include "Error.h"
#include <SDL3/SDL_messagebox.h>

void Error::ShowError(const std::string& title, const std::string& body) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(),body.c_str(), nullptr);
}
