#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

class SDL_FRect;

class RenderManager {
    public:
    int init();

    RenderManager();

    void present() const;
    void clear() const;
    void drawRect(SDL_FRect rect, int color) const;
    void getWindowSize(int *width, int *height) const;
    void checkExit(bool *exit) ;
    void exit();
    private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
};



#endif //RENDERMANAGER_H
