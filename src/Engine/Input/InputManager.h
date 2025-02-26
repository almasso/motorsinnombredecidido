#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

struct InputState {
    float mouse_x = 0.0f;
    float mouse_y = 0.0f;
    bool mouse_down = false;
    bool mouse_up = false;
    bool mouse_pressed = false;
    bool exit = false;
};

class InputManager {
    InputState input_state;
    static InputManager* instance;
    InputManager();
    public:
    static InputManager* Init();
    void update();
    void shutdown() const;
    static const InputState& GetState();
};

#endif //INPUTMANAGER_H
