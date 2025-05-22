//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "RenderManager.h"
#include "common/EditorError.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "WindowStack.h"
#include <filesystem>
#include <sol/types.hpp>

#ifdef __APPLE__
#define GetCurrentDir strdup(SDL_GetBasePath())
#else
#define GetCurrentDir SDL_GetCurrentDirectory()
#endif

std::unique_ptr<editor::render::RenderManager> editor::render::RenderManager::_instance = nullptr;

bool editor::render::RenderManager::Init(uint32_t width, uint32_t height) {
    editorAssert(_instance == nullptr, "Render manager singleton instance is already initialized")
    _instance = std::unique_ptr<RenderManager>(new RenderManager());
    if(!_instance->init(width, height)) {
        Destroy();
        return false;
    }
    return true;
}

bool editor::render::RenderManager::init(uint32_t width, uint32_t height) {
    _width = width;
    _height = height;
    bool initialized = true;
    initialized &= initSDL();
    // If SDL has not been initialized, we discard the initialization of DearImGui, as it needs SDL to work.
    // Si SDL no se ha inicializado, descartamos la inicialización de DearImGui, ya que necesita SDL para funcionar.
    if(initialized)
        initialized &= initDearImGui();
    return initialized;
}

bool editor::render::RenderManager::initSDL() {
    if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        showError(SDL_GetError())
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::SDL_INIT_CORRECT;
    _window = SDL_CreateWindow("", _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if(_window == nullptr) {
        showError(SDL_GetError())
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::SDL_WINDOW_CREATED;
    _renderer = SDL_CreateRenderer(_window, nullptr);
    if(_renderer == nullptr) {
        showError(SDL_GetError())
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::SDL_RENDERER_CREATED;
    SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(_window);
    return true;
}

bool editor::render::RenderManager::initDearImGui() {
    IMGUI_CHECKVERSION();
    _context = ImGui::CreateContext();
    if(_context == nullptr) {
        showError("Couldn't create DearImGui context")
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_CONTEXT_CREATED;
    ImGui::StyleColorsClassic();
    if(!ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer)) {
        showError("Couldn't initialize SDL3 for DearImGui")
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_SDL3_INIT_CORRECT;
    if(!ImGui_ImplSDLRenderer3_Init(_renderer)) {
        showError("Couldn't initialize SDL3 renderer for DearImGui")
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_SDLRENDERER3_INIT_CORRECT;

    _io = &ImGui::GetIO();

    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    _currentDirectory = GetCurrentDir;
    return true;
}

void editor::render::RenderManager::Destroy() {
    _instance.reset(nullptr);
}

editor::render::RenderManager &editor::render::RenderManager::GetInstance() {
    editorAssert(_instance != nullptr, "Render manager singleton instance is not initialized")
    return *_instance;
}

editor::render::RenderManager::~RenderManager() {
    SDL_free(_currentDirectory);
    _currentDirectory = nullptr;
    _io->Fonts->ClearFonts();
    _fonts.clear();
    _io = nullptr;
    if(_initializationSteps & (uint8_t)RenderManager_InitializationSteps::IMGUI_SDLRENDERER3_INIT_CORRECT)
        ImGui_ImplSDLRenderer3_Shutdown();
    if(_initializationSteps & (uint8_t)RenderManager_InitializationSteps::IMGUI_SDL3_INIT_CORRECT)
        ImGui_ImplSDL3_Shutdown();
    if(_initializationSteps & (uint8_t)RenderManager_InitializationSteps::IMGUI_CONTEXT_CREATED) {
        ImGui::DestroyContext(_context);
        _context = nullptr;
    }
    if(_initializationSteps & (uint8_t)RenderManager_InitializationSteps::SDL_RENDERER_CREATED) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if(_initializationSteps & (uint8_t)RenderManager_InitializationSteps::SDL_WINDOW_CREATED) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    if(_initializationSteps & (uint8_t)RenderManager_InitializationSteps::SDL_INIT_CORRECT)
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    _initializationSteps = 0;
}

bool editor::render::RenderManager::render() {
    _checkFontRequests();
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    WindowStack::renderWindows();
    ImGui::Render();
    if(!SDL_SetRenderDrawColorFloat(_renderer, 0.45f, 0.55f, 0.60f, 1.00f)) {
        showError(SDL_GetError())
        return false;
    }
    if(!SDL_RenderClear(_renderer)) {
        showError(SDL_GetError())
        return false;
    }
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
    if(!SDL_RenderPresent(_renderer)) {
        showError(SDL_GetError())
        return false;
    }
    return true;
}

void editor::render::RenderManager::_loadFont(const std::string& name, const std::filesystem::path &file, float size, const ImFontConfig* config, const ImWchar* ranges) {
    if(ranges == nullptr) ranges = _io->Fonts->GetGlyphRangesDefault();
    _fonts[name] = _io->Fonts->AddFontFromFileTTF((std::string(_currentDirectory) + file.string()).c_str(), size, config, ranges);
    if(_fonts[name] == nullptr) {
        showWarning("Failed to load font: " + file.string())
        return;
    }
    if(!_io->Fonts->Build()) {
        showWarning("Failed to build font: " + file.string())
    }
}

void editor::render::RenderManager::_setWindowName(const std::string &name) {
    if(!SDL_SetWindowTitle(_window, name.c_str())) {
        showError(SDL_GetError())
    }
}

void editor::render::RenderManager::setDefaultFont(ImFont *font) {
    _io->FontDefault = font;
}

void editor::render::RenderManager::setWidth(uint32_t width) {
    _width = width;
    if(!SDL_SetWindowSize(_window, _width, _height)) {
        showError(SDL_GetError())
    }
}

void editor::render::RenderManager::setHeight(uint32_t height) {
    _height = height;
    if(!SDL_SetWindowSize(_window, _width, _height)) {
        showError(SDL_GetError())
    }
}

bool editor::render::RenderManager::isWindowShown() const {
    SDL_WindowFlags fl = SDL_GetWindowFlags(_window);
    return !(fl & SDL_WINDOW_HIDDEN) && !(fl & SDL_WINDOW_MINIMIZED);
}

void editor::render::RenderManager::showWindow() {
    if(!SDL_ShowWindow(_window)) {
        showError(SDL_GetError())
    }
}

void editor::render::RenderManager::hideWindow() {
    if(!SDL_HideWindow(_window)) {
        showError(SDL_GetError())
    }
}

void editor::render::RenderManager::centerWindowInScreen() {
    if(!SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
        showError(SDL_GetError())
    }
}

void editor::render::RenderManager::enableWindowResizing() {
    if(!SDL_SetWindowResizable(_window, true)) {
        showError(SDL_GetError())
    }
}

void editor::render::RenderManager::disableWindowResizing() {
    if(!SDL_SetWindowResizable(_window, false)) {
        showError(SDL_GetError())
    }
}

std::string editor::render::RenderManager::getWindowName() const {
    return SDL_GetWindowTitle(_window);
}

void editor::render::RenderManager::updateDimensions() {
    int w, h;
    if(!SDL_GetWindowSize(_window, &w, &h)) {
        showError(SDL_GetError())
    }
    _width = w;
    _height = h;
}

ImTextureID editor::render::RenderManager::_loadTexture(const std::string &filepath) {
    SDL_Texture* texture = IMG_LoadTexture(_renderer, (filepath).c_str());
    if(!texture) {
        showError(SDL_GetError())
        return 0;
    }
    return (ImTextureID)texture;
}

void editor::render::RenderManager::destroyTexture(ImTextureID textureID) {
    SDL_DestroyTexture(reinterpret_cast<SDL_Texture *>(textureID));
}

void editor::render::RenderManager::_checkFontRequests() {
    if (_fontRequest.needsReload) {
        std::string key = _fontRequest.path + std::to_string(_fontRequest.size);
        if (_fonts.contains(key)) {
            *_fontRequest.font = _fonts[key];
        }
        else {
            ImFontConfig cfg;
            cfg.SizePixels = _fontRequest.size;
            cfg.GlyphRanges = _io->Fonts->GetGlyphRangesDefault();
            _fonts[key] = _io->Fonts->AddFontFromFileTTF(_fontRequest.path.c_str(), _fontRequest.size, &cfg, cfg.GlyphRanges);
            *_fontRequest.font = _fonts[key];
            if(!_io->Fonts->Build()) {
                showWarning("Failed to build font: " + _fontRequest.path);
            }
            ImGui_ImplSDLRenderer3_DestroyDeviceObjects();
            ImGui_ImplSDLRenderer3_CreateDeviceObjects();
        }
        _fontRequest.needsReload = false;
    }
}

void editor::render::RenderManager::requestFont(const std::filesystem::path &path, float size, ImFont *&font) {
    _fontRequest.path = path.empty() ? std::string(_currentDirectory) + "settings/fonts/Raleway-Regular.ttf" : path.string();
    _fontRequest.size = size;
    _fontRequest.font = &font;
    _fontRequest.needsReload = true;
}

