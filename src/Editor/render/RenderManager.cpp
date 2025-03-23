//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "RenderManager.h"
#include <cassert>
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "WindowStack.h"

std::unique_ptr<editor::render::RenderManager> editor::render::RenderManager::_instance = nullptr;

bool editor::render::RenderManager::Init(uint32_t width, uint32_t height) {
    assert(_instance == nullptr && "Render manager singleton instance is already initialized || La instancia del singleton del gestor de render ya está inicializada");
    _instance = std::unique_ptr<RenderManager>(new RenderManager());
    if(!_instance->init(width, height)) {
        _instance.reset(nullptr);
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
        SDL_GetError();
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::SDL_INIT_CORRECT;
    _window = SDL_CreateWindow("", _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if(_window == nullptr) {
        SDL_GetError();
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::SDL_WINDOW_CREATED;
    _renderer = SDL_CreateRenderer(_window, nullptr);
    if(_renderer == nullptr) {
        SDL_GetError();
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
    if(_context == nullptr)
        return false;
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_CONTEXT_CREATED;
    ImGui::StyleColorsDark();
    if(!ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer))
        return false;
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_SDL3_INIT_CORRECT;
    if(!ImGui_ImplSDLRenderer3_Init(_renderer))
        return false;
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_SDLRENDERER3_INIT_CORRECT;

    _io = &ImGui::GetIO();

    ImGuiStyle& style = ImGui::GetStyle();
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    return true;
}

editor::render::RenderManager &editor::render::RenderManager::GetInstance() {
    assert(_instance != nullptr);
    return *_instance;
}

editor::render::RenderManager::~RenderManager() {
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

void editor::render::RenderManager::render() {
    SDL_SetWindowSize(_window, _width, _height);
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    WindowStack::renderWindows();
    ImGui::Render();
    SDL_SetRenderDrawColorFloat(_renderer, 0.45f, 0.55f, 0.60f, 1.00f);
    SDL_RenderClear(_renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
    SDL_RenderPresent(_renderer);
}

void editor::render::RenderManager::_loadFont(const std::string& name, const std::filesystem::path &file, float size, const ImFontConfig* config, const ImWchar* ranges) {
    if(ranges == nullptr) ranges = _io->Fonts->GetGlyphRangesDefault();
    _fonts[name] = _io->Fonts->AddFontFromFileTTF(file.string().c_str(), size, config, ranges);
    _io->Fonts->Build();
}

void editor::render::RenderManager::_setWindowName(const std::string &name) {
    SDL_SetWindowTitle(_window, name.c_str());
}

void editor::render::RenderManager::setDefaultFont(ImFont *font) {
    _io->FontDefault = font;
}


