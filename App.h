#pragma once

#include "proto/proto.h"

class App {
public:
    App();
    void Render();

private:
    Color perPixel(double s, double t) const;

private:
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;
    SDL_Event m_event{};

    Image m_image;

    Aggregate m_aggregate;

    int m_t = 0;

};
