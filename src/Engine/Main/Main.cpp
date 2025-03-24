#include "Main.h"

#include <Audio/AudioManager.h>
#include <Collisions/CollisionManager.h>
#include <Core/SceneManager.h>
#include <Input/InputManager.h>
#include <Load/LuaManager.h>
#include <Render/RenderManager.h>
#include <Utils/Rect.h>
#include <Utils/TimeManager.h>
#include <Load/ResourceManager.h>

RenderManager* Main::_render = nullptr;
AudioManager* Main::_audio = nullptr;
TimeManager* Main::_time = nullptr;
InputManager* Main::_input = nullptr;
CollisionManager* Main::_collisions = nullptr;
SceneManager* Main::_scenes = nullptr;

bool Main::Init() {
    _render = new RenderManager();
    if (!_render->init(1280, 720))
        return false;
    _input = InputManager::Init();
    if (!_input)
        return false;
    if (!AudioManager::Init())
        return false;
    _audio = AudioManager::Instance();
    CollisionManager::Init();
    _collisions = CollisionManager::Instance();
    _time = new TimeManager();
    _time->init();
    std::string startScene;
    if (!ResourceManager::Init("assets/config.lua",startScene))
        return false;
    if (!SceneManager::Init(startScene))
        return false;
    _audio->initTest();
    return true;
}
void Main::Shutdown() {
    _audio->shutdownTest();
    ResourceManager::Shutdown();
    delete _time;
    CollisionManager::Shutdown();
    AudioManager::Shutdown();
    _input->shutdown();
    _render->shutdown();
    delete _render;
}

int Main::Loop() {
    if (!Init())
        return 1;

    auto [color, width, height, speed] = LuaManager::loadSquare();
    Rect rect = {250,150,width,height};
    int w, h;
    _render->getWindowSize(&w, &h);
    float dirX = -0.1f, dirY = 0.1f;
    while(!InputManager::GetState().exit) {
        _time->update();
        _input->update();
        _audio->updateTest();
        _collisions->fixedUpdate();
        _render->clear();
        _render->drawRect(rect, color);
        _render->present();
        rect.x -= dirX * speed;
        if(rect.x < 0 || rect.x + rect.w > static_cast<float>(w)) dirX *= -1;
        rect.y += dirY * speed;
        if(rect.y < 0 || rect.y + rect.h > static_cast<float>(h)) dirY *= -1;
    }
    Shutdown();
    return 0;
}
