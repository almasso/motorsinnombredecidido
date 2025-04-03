#ifndef COLOR_H
#define COLOR_H
#include <SDL3/SDL_pixels.h>

struct Color {
    uint8_t r, g, b, a;

    Color() {
        r =  g = b = a = 255;
    }

    Color(const int color) {
       r = color >> 24, g = color >> 16 , b = color >> 8, a = color;
    }

    operator SDL_Color() const { return {r,g,b,a}; }

    bool operator==(const Color & color) const {
        return r == color.r && g == color.g && b == color.b;
    };
};

#endif //COLOR_H
