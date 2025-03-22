#ifndef ENGINE_LIBRARY_H
#define ENGINE_LIBRARY_H

class CollisionManager;
class RenderManager;
class AudioManager;
class InputManager;
class TimeManager;
class ResourceMemoryManager;

class Main {
private:
  	static RenderManager* render;
  	static AudioManager* audio;
  	static TimeManager* time;
  	static InputManager* input;
  	static CollisionManager* collisions;

	static bool Init();
    static void Shutdown();
public:
    static int Loop();
};
#endif //ENGINE_LIBRARY_H