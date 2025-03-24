#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

class RenderManager;

struct InputState {
    float mouse_x = 0.0f;
    float mouse_y = 0.0f;
    bool mouse_down = false;
    bool mouse_up = false;
    bool mouse_pressed = false;
    bool exit = false;
};

class InputManager {
    InputState _inputState;
    static InputManager* _instance;
    InputManager();
    public:
    static InputManager* Init();
    void update(const int& width, const int& height);
    void shutdown() const;
    static const InputState& GetState();
};

#endif //INPUTMANAGER_H
