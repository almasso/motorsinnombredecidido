//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#ifndef RPGBAKER_MAPWIZARD_H
#define RPGBAKER_MAPWIZARD_H

#include "render/ModalWindow.h"

namespace editor {
    class Project;
}

namespace editor::resources {
    class Map;
}

namespace editor::render::modals {
    class MapWizard : public ModalWindow {
    public:
        MapWizard(editor::Project* project);

        void setMapToModify(editor::resources::Map* map, bool modify = false);

        ~MapWizard();
    private:
        editor::Project* _project = nullptr;

        editor::resources::Map* _mapToModify = nullptr;

        bool _isGivingName = false;

        bool _modify = false;

        bool _sameName = false;

        char _nameBuffer[256];

        int _dimensions[2];
        int _layers;

        void beforeRender() override;

        void onRender() override;
    };
}


#endif //RPGBAKER_MAPWIZARD_H
