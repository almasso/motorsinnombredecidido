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
    if (!ResourceManager::Init("data/config.lua",startScene))
        return false;
    _scenes = SceneManager::Init(startScene);
    if (!_scenes)
        return false;
    return true;
}
void Main::Shutdown() {
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

    int w, h;
    while(!InputManager::GetState().exit) {
        _render->getWindowSize(&w, &h);
        _time->update();
        _input->update(w,h);
        _collisions->fixedUpdate();
        if (!_scenes->update())
            return 1;
        _render->clear();
        if (!_scenes->render(_render))
            return 1;
        _render->present();
        _scenes->refresh();
    }
    Shutdown();
    return 0;
}
