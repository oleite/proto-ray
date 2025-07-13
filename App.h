#pragma once

#include "proto/Image.h"
#include <SDL.h>
#include <vector>
#include "proto/proto.h"

class App {
public:
    App();
    void Render();

private:
    static Color perPixel(double s, double t);

private:
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;
    SDL_Event m_event{};

    Image m_image;

    int m_t = 0;

};
