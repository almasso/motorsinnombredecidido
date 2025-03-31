//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_SETTINGSMODAL_H
#define MOTORSINNOMBREDECIDIDO_SETTINGSMODAL_H

#include "render/ModalWindow.h"
#include <vector>

namespace editor::render::modals {
    class SettingsModal : public ModalWindow {
    public:
        SettingsModal();
    private:
        int _languagesSelection = 0;
        std::vector<const char*> _languages;
        std::vector<std::string> _languagesID;

        void onRender() override;
        void beforeRender() override;
        void reset();
    };
}


#endif //MOTORSINNOMBREDECIDIDO_SETTINGSMODAL_H
