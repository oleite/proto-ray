//
// Created by Gabriel Leite on 03/07/25.
//

#include "App.h"

App::App()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int width = 640;
    int height = 480;

    m_window = SDL_CreateWindow(
        "title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    m_image.Initialize(width, height, m_renderer);

    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&m_event)) {
            if (m_event.type == SDL_QUIT) {
                quit = true;
            }
        }

        Render();
    }
}

void App::Render()
{
    m_t++;

    SDL_SetRenderDrawColor(m_renderer, 255, 0, 200, 255);
    SDL_RenderClear(m_renderer);

    int width = 640;
    int height = 480;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double s = (static_cast<double>(x) / width);
            double t = (static_cast<double>(y) / height);

            m_image.SetPixel(x, y, perPixel(s, t));
        }
    }

    m_image.Display();

    SDL_RenderPresent(m_renderer);
}

Color App::perPixel(double s, double t)
{
    // s -= .5;
    // t -= .5;
    //
    //
    // Vector3 sphereCenter{0.0, 0.0, -1.0};
    // float sphereRadius = 0.25;
    //
    // float fov{0.5};
    // Ray camRay{{0, 0, 0}, {static_cast<float>(s)*fov, static_cast<float>(t)*fov, -1}};
    //
    //
    //
    //

    const CheckerboardTexture tex{};
    return tex.sample(s, t);


    return {0.0, 0.1, 0.2};
}