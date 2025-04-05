//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef MOTORSINNOMBREDECIDIDO_TILESETWIZARD_H
#define MOTORSINNOMBREDECIDIDO_TILESETWIZARD_H

#include "render/ModalWindow.h"

namespace editor::render::modals {
    class TilesetWizard : public ModalWindow {
    public:
        TilesetWizard();

    private:
        int _dimensions[2];

        bool _isGivingName = false;

        char _nameBuffer[256];

        char _routeBuffer[1024];

        char _fullRoute[1280];

        void beforeRender() override;

        void onRender() override;
    };
}


#endif //MOTORSINNOMBREDECIDIDO_TILESETWIZARD_H
