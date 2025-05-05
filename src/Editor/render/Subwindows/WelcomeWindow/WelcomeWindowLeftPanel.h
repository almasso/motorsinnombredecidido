//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_WELCOMEWINDOWLEFTPANEL_H
#define RPGBAKER_WELCOMEWINDOWLEFTPANEL_H

#include "render/Subwindow.h"

namespace editor::render::modals {
    class SettingsModal;
}

namespace editor::render::subwindows {
    class WelcomeWindowLeftPanel : public Subwindow {
    public:
        WelcomeWindowLeftPanel(editor::render::modals::SettingsModal* settings);
    private:
        editor::render::modals::SettingsModal* _settings;

        void beforeRender() override;

        void onRender() override;
    };
}


#endif //RPGBAKER_WELCOMEWINDOWLEFTPANEL_H
