#ifndef ENGINE_LIBRARY_H
#define ENGINE_LIBRARY_H

class RenderManager;
class AudioManager;
class InputManager;
class TimeManager;

class Main {
private:
  	static RenderManager* render;
  	static AudioManager* audio;
  	static TimeManager* time;
  	static InputManager* input;

	static bool Init();
    static void Shutdown();
public:
    static int Loop();
};
#endif //ENGINE_LIBRARY_H