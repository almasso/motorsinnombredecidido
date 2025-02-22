#include "Main.h"

#include <Audio/AudioManager.h>
#include <Input/InputManager.h>
#include <Load/LuaManager.h>
#include <Render/RenderManager.h>
#include <Utils/Rect.h>
#include <Utils/TimeManager.h>

RenderManager* Main::render = nullptr;
InputManager* Main::input = nullptr;
AudioManager* Main::audio = nullptr;
TimeManager* Main::time = nullptr;

bool Main::Init() {
    render = new RenderManager();
    input = new InputManager();
    time = new TimeManager();
    if (!render->init(1280, 720))
        return false;
    if (!input->init())
        return false;
    time->init();
    return true;
}
void Main::Shutdown() {
    input->shutdown();
    render->shutdown();
    delete time;
    delete input;
    delete render;
}

int Main::Loop() {
    if (!Init())
        return 1;

    auto [color, width, height, speed] = LuaManager::loadSquare();
    Rect rect = {250,150,width,height};
    int w, h;
    render->getWindowSize(&w, &h);
    float dirX = -0.1f, dirY = 0.1f;
    while(!input->checkExit()) {
        time->update();
        render->clear();
        render->drawRect(rect, color);
        render->present();
        rect.x -= dirX * speed;
        if(rect.x < 0 || rect.x + rect.w > static_cast<float>(w)) dirX *= -1;
        rect.y += dirY * speed;
        if(rect.y < 0 || rect.y + rect.h > static_cast<float>(h)) dirY *= -1;
    }
    Shutdown();
    return 0;
}
