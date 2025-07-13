#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include "proto.h"

class Image
{
public:
    Image();
    ~Image();

    void Initialize(const int width, const int height, SDL_Renderer* pRenderer);

    inline constexpr void SetPixel(const int x, const int y, const Color &c)
    {
        m_pixels[y * m_width + x] = c.convert();
    }

    void Display();

private:
    void InitTexture();

private:
    std::vector<Uint32> m_pixels;

    int m_width, m_height;

    SDL_Renderer* m_pRenderer;
    SDL_Texture* m_pTexture;
};
