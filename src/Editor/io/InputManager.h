//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_INPUTMANAGER_H
#define MOTORSINNOMBREDECIDIDO_INPUTMANAGER_H

#include <imgui.h>
#include <memory>

namespace editor::io {
    class InputManager {
    public:
        static bool Init();

        static InputManager &GetInstance();

        void handleInput();

        [[nodiscard]] bool quit() const;

        InputManager(const InputManager &) = delete;

        InputManager &operator=(const InputManager &) = delete;

        ~InputManager();
    private:
        static std::unique_ptr<InputManager> _instance;

        bool _quitSignal = false;

        ImGuiIO* _io = nullptr;

        InputManager() = default;

        bool init();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_INPUTMANAGER_H
