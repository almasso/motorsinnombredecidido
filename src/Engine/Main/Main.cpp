#include "Main.h"

#include <Audio/AudioManager.h>
#include <Input/InputManager.h>
#include <Load/LuaManager.h>
#include <Render/RenderManager.h>
#include <Utils/Rect.h>
#include <Utils/TimeManager.h>
#include <Load/ResourceMemoryManager.h>

RenderManager* Main::render = nullptr;
AudioManager* Main::audio = nullptr;
TimeManager* Main::time = nullptr;
InputManager* Main::input = nullptr;
ResourceMemoryManager* Main::resourceMemory = nullptr;

bool Main::Init() {
    render = new RenderManager();
    if (!render->init(1280, 720))
        return false;
    input = InputManager::Init();
    if (!input)
        return false;
    resourceMemory = new ResourceMemoryManager(1024*1024*1024);
    if (!AudioManager::Init(resourceMemory))
        return false;
    audio = AudioManager::Instance();
    time = new TimeManager();
    time->init();
    return true;
}
void Main::Shutdown() {
    AudioManager::Shutdown();
    render->shutdown();
    input->shutdown();
    delete time;
    delete audio;
    delete resourceMemory;
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
    while(!InputManager::GetState().exit) {
        time->update();
        input->update();
        audio->update();
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
