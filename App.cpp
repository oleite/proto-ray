//
// Created by Gabriel Leite on 03/07/25.
//

#include "App.h"

App::App()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    constexpr int width = 640;
    constexpr int height = 480;

    m_window = SDL_CreateWindow(
        "title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    m_image.initialize(width, height, m_renderer);

    auto sphere = std::make_shared<Sphere>(Vector3{0.0, 0.0, -3.0}, 1.0);
    m_aggregate.primitives.push_back(sphere);

    m_aggregate.primitives.push_back(std::make_shared<Sphere>(Vector3{-1.5, 1.0, -4.0}, 1.0));

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

    // SDL_SetRenderDrawColor(m_renderer, 255, 0, 200, 255);
    // SDL_RenderClear(m_renderer);

    for (int x = 0; x < m_image.width; x++) {
        for (int y = 0; y < m_image.height; y++) {
            const double s = (static_cast<double>(x) / m_image.width);
            const double t = (static_cast<double>(y) / m_image.height);
            m_image.setPixel(x, y, perPixel(s, t));
        }
    }

    m_image.display();

    SDL_RenderPresent(m_renderer);
}

Color App::perPixel(const double s, const double t) const
{
    const double ratio = static_cast<double>(m_image.width) / m_image.height;
    const double cu = (s - 0.5) * ratio;
    const double cv = t - 0.5;

    constexpr double fov{1.0};
    Ray camRay{
        {0, 0, 1.0},
        {cu * fov, cv * fov, -1.0}
    };

    //sphere->center.y = sin(t * 0.05);

    const CheckerboardTexture tex{};

    const Intersection i = m_aggregate.intersect(camRay);
    if (i.hit) {
        const double intensity = std::max(0.0, dot(normalize({1, -1, 1}), i.normal));
        return tex.sample(i.u, i.v) * intensity;
    }

    return {0.0, 0.1, 0.2};
}