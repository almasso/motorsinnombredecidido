#include "MainLoop.h"
#include <Input/InputManager.h>
#include <Load/LuaManager.h>
#include <Render/RenderManager.h>
#include <Utils/Rect.h>


void MainLoop::loop() {
    auto render = RenderManager();
    render.init(1280, 720);
    auto input = InputManager();
    input.init();
    auto [color, width, height, speed] = LuaManager::loadSquare();
    Rect rect = {250,150,width,height};
    int w, h;
    render.getWindowSize(&w, &h);
    float dirX = -0.1f, dirY = 0.1f;
    while(!input.checkExit()) {
        render.clear();
        render.drawRect(rect, color);
        render.present();
        rect.x -= dirX * speed;
        if(rect.x < 0 || rect.x + rect.w > static_cast<float>(w)) dirX *= -1;
        rect.y += dirY * speed;
        if(rect.y < 0 || rect.y + rect.h > static_cast<float>(h)) dirY *= -1;
    }
    render.shutdown();
    input.shutdown();


}