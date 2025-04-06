//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_MAPWIZARD_H
#define MOTORSINNOMBREDECIDIDO_MAPWIZARD_H

#include "render/ModalWindow.h"

namespace editor::render::modals {
    class MapWizard : public ModalWindow {
    public:
        MapWizard();

        ~MapWizard();
    private:
        bool _isGivingName = false;

        char _nameBuffer[256];

        int _dimensions[2];

        void beforeRender() override;

        void onRender() override;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_MAPWIZARD_H
