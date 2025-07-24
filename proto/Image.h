#pragma once

#include "proto.h"
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

class Image
{
public:
    int width, height;

public:
    Image()
        : width(0)
        , height(0)
        , m_pRenderer(nullptr)
        , m_pTexture(nullptr)
    {}

    ~Image()
    {
        if (m_pTexture != nullptr)
            SDL_DestroyTexture(m_pTexture);
    }

    void initialize(const int w, const int h, SDL_Renderer *renderer)
    {
        width = w;
        height = h;
        m_pRenderer = renderer;
        m_pixels.resize(width * height);
        initTexture();
    }

    inline void setPixel(const int x, const int y, const Color &c)
    {
        m_pixels[y * width + x] = c.convert();
    }

    void display() const;

private:
    void initTexture();

private:
    std::vector<Uint32> m_pixels;

    SDL_Renderer *m_pRenderer;
    SDL_Texture *m_pTexture;
};
