#ifndef ENGINE_LIBRARY_H
#define ENGINE_LIBRARY_H

class CollisionManager;
class RenderManager;
class AudioManager;
class InputManager;
class TimeManager;
class SceneManager;

class Main {
private:
  	static RenderManager* _render;
  	static AudioManager* _audio;
  	static TimeManager* _time;
  	static InputManager* _input;
  	static CollisionManager* _collisions;
  	static SceneManager* _scenes;

	static bool Init();
    static void Shutdown();
public:
    static int Loop();
};
#endif //ENGINE_LIBRARY_H