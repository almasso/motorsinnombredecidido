#include "MainLoop.h"
#include <SDL3/SDL.h>
#include <iostream>

#include "LuaManager.h"
#include "RenderManager.h"
#include "AssetManager.h"

void MainLoop::loop() {
    RenderManager render = RenderManager();
    auto square = LuaManager::loadSquare();
    SDL_FRect rect = {250,150,square.width,square.height};
    int w, h;
    render.getWindowSize(&w, &h);
    bool exit = false;
    float dirX = -0.1f, dirY = 0.1f;
    while(!exit) {
        render.clear();
        render.drawRect(rect, square.color);
        render.present();
        render.checkExit(&exit);
        rect.x -= dirX * square.speed;
        if(rect.x < 0 || rect.x + rect.w > static_cast<float>(w)) dirX *= -1;
        rect.y += dirY * square.speed;
        if(rect.y < 0 || rect.y + rect.h > static_cast<float>(h)) dirY *= -1;
    }
    render.exit();
}
