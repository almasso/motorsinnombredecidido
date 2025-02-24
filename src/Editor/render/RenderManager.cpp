//
// MIT License
// Copyright (c) 2025 Alejandro Massó Martínez, Miguel Curros García, Alejandro González Sánchez
//

#include "RenderManager.h"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "WindowStack.h"

bool editor::render::RenderManager::init() {
    bool initialized = true;
    initialized &= initSDL();
    // If SDL has not been initialized, we discard the initialization of DearImGui, as it needs SDL to work.
    // Si SDL no se ha inicializado, descartamos la inicialización de DearImGui, ya que necesita SDL para funcionar.
    if(initialized)
        initialized &= initDearImGui();
    return initialized;
}

void editor::render::RenderManager::render() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // Llamadas a WindowStack

    ImGui::Render();
    SDL_SetRenderDrawColorFloat(_renderer, 0.45f, 0.55f, 0.60f, 1.00f);
    SDL_RenderClear(_renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
    SDL_RenderPresent(_renderer);
}

editor::render::RenderManager::~RenderManager() {
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

bool editor::render::RenderManager::initSDL() {
    if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_GetError();
        return false;
    }
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::SDL_INIT_CORRECT;
    _window = SDL_CreateWindow("Ngin Editor", 120, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
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
    ImGui::StyleColorsLight();
     if(!ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer))
         return false;
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_SDL3_INIT_CORRECT;
     if(!ImGui_ImplSDLRenderer3_Init(_renderer))
         return false;
    _initializationSteps |= (uint8_t)RenderManager_InitializationSteps::IMGUI_SDLRENDERER3_INIT_CORRECT;
     return true;
}
