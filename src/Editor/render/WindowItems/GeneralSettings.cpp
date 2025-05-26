//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "GeneralSettings.h"

#include <common/EditorError.h>
#include <common/Project.h>
#include <io/LocalizationManager.h>
#include <render/RenderManager.h>
#include <resources/Map.h>
#include <resources/Sprite.h>
#include <resources/Tile.h>
#include <utils/tinyfiledialogs/tinyfiledialogs.h>


editor::render::tabs::GeneralSettings::GeneralSettings(editor::Project* project) :
    WindowItem(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings").c_str()),
    _project(project) {
    _filePath = _project->getPath() / "projectfiles" / "settings" / "generalSettings.lua";
    sol::table settings = io::LuaManager::GetInstance().getTable(_filePath.string(), true);
    if (settings.valid()) {
        _gameName = settings["gameName"].get_or(_project->getName());
        _gameIcon = std::filesystem::path(settings["gameIcon"].get_or(std::string()));
        std::string normalizedPath = (_project->getAssetsPath() / _gameIcon).lexically_normal().string();
        _loadedIcon = RenderManager::GetInstance().loadTexture(normalizedPath);
        _startingMap = settings["startingMap"].get_or(std::string());
        sol::object startingPosition = settings["startingPosition"];
        if (startingPosition.valid() && startingPosition.is<sol::table>()){
            sol::table startingPositionTable = startingPosition;
            if (startingPositionTable.size() == 2) {
                for (std::size_t i = 0; i < 2; ++i) {
                    _startingPosition[i] = startingPositionTable[i + 1].get_or(0);
                }
            }
        }
        sol::object cameraSize = settings["cameraSize"];
        if (cameraSize.valid() && cameraSize.is<sol::table>()){
            sol::table cameraSizeTable = cameraSize;
            if (cameraSizeTable.size() == 2) {
                for (std::size_t i = 0; i < 2; ++i) {
                    _cameraSize[i] = cameraSizeTable[i + 1].get_or(18 - 6*i);
                }
            }
        }
        sol::object textColor = settings["textColor"];
        if (textColor.valid() && textColor.is<sol::table>()) {
            sol::table textColorTable = textColor;
            if (textColorTable.size() == 4) {
                for (std::size_t i = 0; i < 4; ++i) {
                    _textColor[i] = textColorTable[i + 1].get_or(i == 3 ? 1.0f : 0.0f);
                }
            }
        }
        sol::object backgroundColor = settings["backgroundColor"];
        if (backgroundColor.valid() && backgroundColor.is<sol::table>()) {
            sol::table backgroundColorTable = backgroundColor;
            if (backgroundColorTable.size() == 4) {
                for (std::size_t i = 0; i < 4; ++i) {
                    _backgroundColor[i] = backgroundColorTable[i + 1].get_or(1.0f);
                }
            }
        }
        _font = std::filesystem::path(settings["font"].get_or(std::string()));
        _fontSize = settings["fontSize"].get_or(24);
        _sliderValue = _fontSize;
        _fontModified = true;
        _masterVolume = settings["masterVolume"].get_or(1.0f);
        _musicVolume = settings["musicVolume"].get_or(0.5f);
        _sfxVolume = settings["sfxVolume"].get_or(1.0f);

#ifdef _WIN32
        std::string sdkContainerDir = getenv("LOCALAPPDATA");
        std::string signerFileName = "apksigner.bat";
#elifdef __APPLE__
        std::string sdkContainerDir = "~/Libary";
        std::string signerFileName = "apksigner";
#elifdef  __linux__
        std::string sdkContainerDir = "~";
        std::string signerFileName = "apksigner";
#endif
        _androidApkSignerPath = sdkContainerDir + "/Android/Sdk/build-tools";
        std::filesystem::directory_iterator dir(_androidApkSignerPath);
        if (dir == std::filesystem::directory_iterator()) _androidApkSignerPath = std::filesystem::path();
        else _androidApkSignerPath = dir->path() / signerFileName;
        _androidApkSignerPath.make_preferred();
        _project->setAndroidApkSignerPath(_androidApkSignerPath);
    }
}

editor::render::tabs::GeneralSettings::~GeneralSettings() {
}

void editor::render::tabs::GeneralSettings::onRender() {
    ImGui::Dummy(ImVec2(RenderManager::GetInstance().getWidth()/4, 0));
    ImGui::SameLine();
    drawSettings();
    if(_somethingModified && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        save();
    }
}

void editor::render::tabs::GeneralSettings::beforeRender() {
    _itemFlags = _somethingModified ? ImGuiTabItemFlags_UnsavedDocument : 0;
}

void editor::render::tabs::GeneralSettings::drawMapPreview(resources::Map * map) {
    const float previewSize = 256.0f;
    ImGui::BeginChild("##mapPreview", ImVec2(previewSize, previewSize), true, ImGuiWindowFlags_NoScrollWithMouse);

    int mapWidth = map->getMapWidth();
    int mapHeight = map->getMapHeight();
    int layers = map->getLayers();

    if (mapWidth == 0 || mapHeight == 0 || layers == 0) {
        ImGui::EndChild();
        return;
    }

    int tileWidth = _project->getDimensions()[0];
    int tileHeight = _project->getDimensions()[1];

    float totalPixelWidth  = static_cast<float>(mapWidth * tileWidth);
    float totalPixelHeight = static_cast<float>(mapHeight * tileHeight);

    float scaleX = previewSize / totalPixelWidth;
    float scaleY = previewSize / totalPixelHeight;
    float scale = std::min(scaleX, scaleY);

    float offsetX = (previewSize - totalPixelWidth * scale) * 0.5f;
    float offsetY = (previewSize - totalPixelHeight * scale) * 0.5f;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    auto& tiles = map->getTiles();

    for (int j = 0; j < mapHeight; ++j) {
        for (int i = 0; i < mapWidth; ++i) {
            ImVec2 tilePos = ImVec2(
                origin.x + offsetX + i * tileWidth * scale,
                origin.y + offsetY + j * tileHeight * scale
            );
            ImVec2 tileEnd = ImVec2(
                tilePos.x + tileWidth * scale,
                tilePos.y + tileHeight * scale
            );

            for (int layer = 0; layer < layers; ++layer) {
                int index = i + mapWidth * j;
                if (layer < tiles.size() && index < tiles[layer].size()) {
                    resources::Tile* tile = tiles[layer][index];
                    if (tile && tile->texture) {
                        drawList->AddImage(
                            tile->texture,
                            tilePos,
                            tileEnd,
                            tile->rect.Min,
                            tile->rect.Max
                        );
                    }
                }
            }
        }
    }

    int selX = _startingPosition[0];
    int selY = _startingPosition[1];
    if (selX >= 0 && selY >= 0 && selX < mapWidth && selY < mapHeight) {
        ImVec2 rectPos = ImVec2(
            origin.x + offsetX + selX * tileWidth * scale,
            origin.y + offsetY + selY * tileHeight * scale
        );
        ImVec2 rectEnd = ImVec2(
            rectPos.x + tileWidth * scale,
            rectPos.y + tileHeight * scale
        );

        drawList->AddRect(
            rectPos,
            rectEnd,
            IM_COL32(255, 255, 0, 255),
            0.0f,
            0,
            2.0f
        );
    }

    ImGui::EndChild();
}

void editor::render::tabs::GeneralSettings::save() {
    _somethingModified = false;
    sol::table settings = io::LuaManager::GetInstance().getState().create_table();
    settings["gameName"] = _gameName;
    settings["gameIcon"] = _gameIcon.string();
    settings["startingMap"] = _startingMap;
    settings["startingPosition"] = sol::as_table<std::array<int,2>>({_startingPosition[0], _startingPosition[1]});
    settings["cameraSize"] = sol::as_table<std::array<int,2>>({_cameraSize[0], _cameraSize[1]});
    settings["textColor"] = sol::as_table<std::array<float,4>>({_textColor[0], _textColor[1],_textColor[2], _textColor[3]});
    settings["backgroundColor"] = sol::as_table<std::array<float,4>>({_backgroundColor[0], _backgroundColor[1],_backgroundColor[2], _backgroundColor[3]});
    settings["font"] = _font.string();
    settings["fontSize"] = _fontSize;
    settings["masterVolume"] = _masterVolume;
    settings["musicVolume"] = _musicVolume;
    settings["sfxVolume"] = _sfxVolume;
    io::LuaManager::GetInstance().writeToFile(settings, _filePath.string());
}

uint32_t colorToHex(const float color[4]) {
    auto toByte = [](float c) -> uint8_t {
        return static_cast<uint8_t>(std::clamp(c, 0.0f, 1.0f) * 255.0f + 0.5f);
    };

    uint8_t r = toByte(color[0]);
    uint8_t g = toByte(color[1]);
    uint8_t b = toByte(color[2]);
    uint8_t a = toByte(color[3]);

    return (r << 24) | (g << 16) | (b << 8) | a;
}

sol::table editor::render::tabs::GeneralSettings::buildOverworldScene(sol::table& playerComponents) const {
    auto& lua = io::LuaManager::GetInstance().getState();
    const auto dimensions = _project->getDimensions();

    sol::table scene = lua.create_table();
    sol::table manager = lua.create_table();
    sol::table components = lua.create_table();
    sol::table movement = lua.create_table();
    sol::table overworld = lua.create_table();
    movement["tileWidth"] = dimensions[0];
    movement["tileHeight"] = dimensions[1];
    components["MovementManager"] = movement;
    overworld["startingMap"] = _startingMap.empty() ? _project->getMaps().begin()->first : _startingMap;
    components["OverworldManager"] = overworld;
    manager["handler"] = "Manager";
    manager["components"] = components;
    scene["manager"] = manager;

    sol::table player = lua.create_table();
    sol::table transform = lua.create_table();
    auto map = _project->getMap(_startingMap);
    transform["position"] = sol::as_table<std::array<int,2>>({
        (map->getMapX() + _startingPosition[0]) * dimensions[0],
        (map->getMapY() + _startingPosition[1]) * dimensions[1]
    });
    playerComponents["Transform"] = transform;
    player["components"] = playerComponents;
    player["handler"] = "Player";
    sol::table camera = lua.create_table();
    sol::table cameraComponent = lua.create_table();
    components = lua.create_table();
    cameraComponent["size"] = sol::as_table<std::array<int,2>>({dimensions[0] * _cameraSize[0], dimensions[1] * _cameraSize[1]});
    components["Camera"] = cameraComponent;
    components["Transform"] = sol::as_table<std::array<int,1>>({0});
    camera["components"] = components;
    camera["handler"] = "Camera";
    sol::table cameraChildren = lua.create_table();

    sol::table textBox = lua.create_table();
    textBox["handler"] = "TextBox";
    textBox["active"] = false;
    sol::table textBoxComponents = lua.create_table();
    sol::table textBoxTransform = lua.create_table();
    textBoxTransform["position"] = sol::as_table<std::array<int,2>>({0,
        static_cast<int>(370.0f/1080.0f * dimensions[1] * _cameraSize[1])});
    textBoxComponents["Transform"] = textBoxTransform;
    sol::table textBoxText = lua.create_table();
    if (usingGenericFont()) {
        textBoxText["font"] = "data/assets/Raleway-Regular.ttf";
    }
    else {
        textBoxText["font"] = "data/assets/" + _font.string();
    }
    textBoxText["fontSize"] = _fontSize;
    textBoxText["color"] = colorToHex(_textColor);
    textBoxText["size"] = sol::as_table<std::array<int,2>>({
        static_cast<int>(1580.0f/1920.0f * dimensions[0] * _cameraSize[0]),
        static_cast<int>(280.0f/1080.0f * dimensions[1] * _cameraSize[1])});
    textBoxText["layer"] = _project->getMaxLayers() + 1;
    textBoxComponents["Text"] = textBoxText;
    sol::table textBoxRectangle = lua.create_table();
    textBoxRectangle["layer"] = _project->getMaxLayers();
    textBoxRectangle["size"] = sol::as_table<std::array<int,2>>({
        static_cast<int>(1600.0f/1920.0f  * dimensions[0] * _cameraSize[0]),
        static_cast<int>(290.0f/1080.0f * dimensions[1] * _cameraSize[1])});
    textBoxRectangle["color"] = colorToHex(_backgroundColor);
    textBoxComponents["Rectangle"] = textBoxRectangle;
    sol::table textBoxTextBox = lua.create_table();
    textBoxTextBox.add(0);
    textBoxComponents["TextBox"] = textBoxTextBox;
    textBox["components"] = textBoxComponents;
    cameraChildren["textBox"] = textBox;

    for (int i = 1; i < 4; i++) {
        sol::table choice = lua.create_table();
        std::string choiceId ="Choice"+std::to_string(i);
        choice["handler"] = choiceId;
        choice["active"] = false;
        sol::table choiceComponents = lua.create_table();
        sol::table choiceTransform = lua.create_table();
        choiceTransform["position"] = sol::as_table<std::array<int,2>>({
            static_cast<int>(400.0f*(i-2)/1920.0f * dimensions[0] * _cameraSize[0]),
            static_cast<int>(100.0f/1080.0f * dimensions[1] * _cameraSize[1])});
        choiceComponents["Transform"] = choiceTransform;
        sol::table choiceText = lua.create_table();
        choiceText["font"] = "data/assets/" + _font.string();
        choiceText["fontSize"] = _fontSize;
        choiceText["color"] = colorToHex(_textColor);
        choiceText["size"] = sol::as_table<std::array<int,2>>({
            static_cast<int>(200.0f/1920.0f * dimensions[0] * _cameraSize[0]),
            static_cast<int>(100.0f/1080.0f * dimensions[1] * _cameraSize[1])});
        choiceText["layer"] = _project->getMaxLayers() + 1;
        choiceComponents["Text"] = choiceText;
        sol::table choiceRectangle = lua.create_table();
        choiceRectangle["layer"] = _project->getMaxLayers();
        choiceRectangle["size"] = sol::as_table<std::array<int,2>>({
            static_cast<int>(220.0f/1920.0f  * dimensions[0] * _cameraSize[0]),
            static_cast<int>(120.0f/1080.0f * dimensions[1] * _cameraSize[1])});
        choiceRectangle["color"] = colorToHex(_backgroundColor);
        choiceComponents["Rectangle"] = choiceRectangle;
        sol::table choiceButton = lua.create_table();
        choiceButton["size"] = sol::as_table<std::array<int,2>>({
            static_cast<int>(220.0f/1920.0f  * dimensions[0] * _cameraSize[0]),
            static_cast<int>(120.0f/1080.0f * dimensions[1] * _cameraSize[1])});
        choiceRectangle["params"] = sol::as_table<std::array<int,1>>({i});
        choiceComponents["Button"] = choiceButton;
        choice["Components"] = choiceComponents;
        cameraChildren[choiceId] = choice;
    }

    camera["children"] = cameraChildren;
    sol::table children = lua.create_table();
    children["camera"] = camera;
    player["children"] = children;
    scene["player"] = player;

    sol::table music = lua.create_table();
    music["handler"] = "Music";
    sol::table musicComponents = lua.create_table();
    sol::table audioSource = lua.create_table();
    audioSource["audio"] = "";
    audioSource["mixer"] = "data/mixers/music.mixer.lua";
    audioSource["loop"] = true;
    musicComponents["AudioSource"] = audioSource;
    music["components"] = musicComponents;
    scene["music"] = music;

    return scene;
}

std::array<float, 3> editor::render::tabs::GeneralSettings::getAudioSettings() const {
    return std::array{_masterVolume, _musicVolume, _sfxVolume};
}

bool editor::render::tabs::GeneralSettings::usingGenericFont() const {
    return _font.empty();
}

void editor::render::tabs::GeneralSettings::drawSettings() {
    ImGui::BeginChild("##settings", ImVec2(RenderManager::GetInstance().getWidth()/2, 0), true);
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.settings").c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::BeginChild("##gamesettings", ImVec2(2 * ImGui::GetContentRegionAvail().x / 3, 256), true);
    ImGui::Dummy(ImVec2(0.0f, 16.0f));
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.gameName").c_str());
    char gameName[256];
    std::strncpy(gameName, _gameName.c_str(), sizeof(gameName) - 1);
    if (ImGui::InputText("##settings", gameName, IM_ARRAYSIZE(gameName))) {
        _gameName = gameName;
        _project->setGameName(gameName);
        _somethingModified = true;
    };
    ImGui::Dummy(ImVec2(0.0f, 32.0f));
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.gameIcon").c_str());
    ImGui::Spacing();
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("action.search").c_str())) {
        const char* fileExtension[] = {"*.stb", "*.bmp", "*.gif", "*.jpg", "*.lbm", "*.pcx", "*.png", "*.pnm", "*.qoi", "*.svg", "*.tga", "*.xcf", "*.xpm", "*.xv"};
        const char* selectedFile = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("action.selectimage").c_str(),
                (_project->getPath() / "assets/").string().c_str(),
                14,
                fileExtension,
                nullptr,
                0
        );
        if(selectedFile != nullptr) {
            std::filesystem::path selectedPath = selectedFile;
            if(selectedPath.parent_path() != _project->getPath() / "assets") {
                showUserWarning(io::LocalizationManager::GetInstance().getString("error.assetlocationnotvalid"))
            }
            else {
                if(_loadedIcon != 0) RenderManager::GetInstance().destroyTexture(_loadedIcon);
                _gameIcon = selectedPath.lexically_relative(_project->getAssetsPath());
                std::string normalizedPath  = std::filesystem::path(selectedFile).lexically_normal().string();
                _loadedIcon = RenderManager::GetInstance().loadTexture(normalizedPath);
                _somethingModified = true;
            }
        }
    }
    ImGui::SameLine();
    char routeBuffer[1024];
    strncpy(routeBuffer, _gameIcon.string().c_str(), sizeof(routeBuffer) - 1);
    ImGui::InputText("##gameIcon", routeBuffer, IM_ARRAYSIZE(routeBuffer),ImGuiInputTextFlags_ReadOnly);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(16.0f, 0.0f));
    ImGui::SameLine();
    constexpr ImVec2 iconSize = ImVec2(256, 256);
    ImGui::Image(_loadedIcon, iconSize);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::BeginChild("##mapSettings", ImVec2(2 * ImGui::GetContentRegionAvail().x / 3, 256), true);
    ImGui::Dummy(ImVec2(0.0f, 16.0f));
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.startingMap").c_str());
    if (ImGui::BeginCombo("##mapDropdown", _startingMap != "" ? _startingMap.c_str() : io::LocalizationManager::GetInstance().getString("window.mainwindow.mapeditor.mapselector").c_str())) {
        for (auto [name, map] : _project->getMaps()) {
            if(ImGui::Selectable((name).c_str(), _startingMap == name)) {
                _startingMap = name;
                _startingPosition[0] = 0;
                _startingPosition[1] = 0;
                _somethingModified = true;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::Dummy(ImVec2(0.0f, 32.0f));
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.startingPosition").c_str());
    int playerPos[2] = {_startingPosition[0], _startingPosition[1]};
    if (ImGui::InputInt2("##playerPos", playerPos)) {
        resources::Map* temp = _project->getMap(_startingMap);
        if (temp && playerPos[0] < temp->getMapWidth() && playerPos[0] >= 0) {
            _startingPosition[0] = playerPos[0];
            _somethingModified = true;
        }
        if (temp && playerPos[1] < temp->getMapHeight() && playerPos[1] >= 0) {
            _startingPosition[1] = playerPos[1];
            _somethingModified = true;
        }
    }
    ImGui::Spacing();
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(16.0f, 0.0f));
    ImGui::SameLine();
    resources::Map* temp = _project->getMap(_startingMap);
    if (temp) drawMapPreview(temp);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.camera").c_str());
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/4);
    if (ImGui::DragInt2(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.cameraSize").c_str(),
                        _cameraSize, 1, 1, 100)) {
        _somethingModified = true;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.text").c_str());
    ImGui::Spacing();
    if (!_font.empty()) {
        ImGui::Text("%s", _font.filename().stem().string().c_str());
        ImGui::SameLine();
    }
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.selectFont").c_str())) {
        const char* fileExtension[] = {"*.ttf"};
        const char* route = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.selectFont").c_str(),
                (_project->getPath() / "assets/").string().c_str(),
                1,
                fileExtension,
                nullptr,
                0
        );
        if(route != nullptr) {
            if(std::filesystem::path(route).parent_path() != (_project->getPath() / "assets")) {
                showUserWarning(io::LocalizationManager::GetInstance().getString("error.assetlocationnotvalid"))
            }
            else {
                std::filesystem::path selectedPath(route);
                _font = selectedPath.lexically_relative(_project->getAssetsPath());
                _fontModified = true;
                _somethingModified = true;
            }
        }
    }
    ImGui::Spacing();
    if(ImGui::ColorEdit4(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.textColor").c_str(), _textColor)) {
        _somethingModified = true;
    }
    ImGui::Spacing();
    if (ImGui::ColorEdit4(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.backgroundColor").c_str(), _backgroundColor)) {
        _somethingModified = true;
    }
    ImGui::Spacing();
    ImGui::SetNextItemWidth(RenderManager::GetInstance().getWidth()/6);
    ImGui::SliderInt(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.fontSize")).c_str(), &_sliderValue, 18, 72);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        _fontModified = true;
        _somethingModified = true;
        _fontSize = _sliderValue;
    }
    if (_fontModified) {
        std::filesystem::path fontPath = _font.empty() ? std::filesystem::path() : _project->getAssetsPath() / _font;
        RenderManager::GetInstance().requestFont(fontPath, static_cast<float>(_fontSize), _previewFont);
        _fontModified = false;
    }
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(
        _backgroundColor[0],
        _backgroundColor[1],
        _backgroundColor[2],
        _backgroundColor[3]
    ));
    ImVec2 childSize(ImGui::GetContentRegionAvail().x, 150.0f);
    ImGui::BeginChild("FontPreviewChild", childSize, true);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(
        _textColor[0],
        _textColor[1],
        _textColor[2],
        _textColor[3]
    ));
    if (_previewFont)
        ImGui::PushFont(_previewFont);
    float textHeight = static_cast<float>(_fontSize);
    ImGui::SetCursorPosY((childSize.y - textHeight) * 0.5f);
    ImGui::Text(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.textPreview").c_str());
    if (_previewFont)
        ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("%s", io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.audio").c_str());
    if(ImGui::SliderFloat(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.masterVolume")).c_str(),
        &_masterVolume, 0, 1)) {_somethingModified = true;}
    ImGui::Spacing();
    if(ImGui::SliderFloat(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.musicVolume")).c_str(),
        &_musicVolume, 0, 1)) {_somethingModified = true;}
    ImGui::Spacing();
    if(ImGui::SliderFloat(("   " + io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.sfxVolume")).c_str(),
        &_sfxVolume, 0, 1)) {_somethingModified = true;}
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Configuración de build");
    ImGui::Spacing();
    drawApkSignerPathSelector();
    ImGui::EndChild();
}
#define MAX_SIGNER_BUFFER 1024
void editor::render::tabs::GeneralSettings::drawApkSignerPathSelector() {
    char* buffer = new char[MAX_SIGNER_BUFFER];
    bool assigned = false;
    if(ImGui::Button(io::LocalizationManager::GetInstance().getString("action.search").c_str())) {
        const char* fileExtension[] = {"*"};
        const char* route = tinyfd_openFileDialog(
                io::LocalizationManager::GetInstance().getString("action.selectapksigner").c_str(),
                (_androidApkSignerPath).string().c_str(),
                1,
                fileExtension,
                nullptr,
                0
        );
        if(route != nullptr) {
            assigned = true;
            std::string fR(std::filesystem::path(route).lexically_normal().string());
            strncpy(buffer, fR.c_str(), MAX_SIGNER_BUFFER - 1);
            buffer[MAX_SIGNER_BUFFER - 1] = '\0';
            _somethingModified = true;
        }
        else {
            _androidApkSignerPath.string().copy(buffer, MAX_SIGNER_BUFFER - 1);
            buffer[_androidApkSignerPath.string().size()] = '\0';
        }
    }
    else {
        _androidApkSignerPath.string().copy(buffer, MAX_SIGNER_BUFFER - 1);
        buffer[_androidApkSignerPath.string().size()] = '\0';
    }
    ImGui::SameLine();
    if(ImGui::InputText(io::LocalizationManager::GetInstance().getString("window.mainwindow.generalSettings.apkSignerPath").c_str(),
                     buffer, MAX_SIGNER_BUFFER - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
        _somethingModified = true;
    }
    if (assigned) {
        _androidApkSignerPath = buffer;
    }
    delete[] buffer;
    _project->setAndroidApkSignerPath(_androidApkSignerPath);
}
