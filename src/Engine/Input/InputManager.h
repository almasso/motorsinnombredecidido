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
    static InputState input_state;
    public:
    static bool init();
    static void update();
    static void shutdown();
    static const InputState& getState();
};

#endif //INPUTMANAGER_H
