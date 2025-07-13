//
// Created by Gabriel Leite on 09/07/25.
//

#include "Image.h"

Image::Image()
{
    m_width = 0;
    m_height = 0;
    m_pTexture = nullptr;
}

Image::~Image()
{
    if (m_pTexture != nullptr)
        SDL_DestroyTexture(m_pTexture);
}

void Image::Initialize(const int width, const int height, SDL_Renderer *pRenderer)
{
    m_width = width;
    m_height = height;

    m_pixels.resize(m_width * m_height);

    m_pRenderer = pRenderer;

    InitTexture();
}


void Image::Display()
{
    SDL_UpdateTexture(m_pTexture, nullptr, m_pixels.data(), m_width * sizeof(Uint32));

    const SDL_Rect rect = {0, 0, m_width, m_height};
    SDL_RenderCopy(m_pRenderer, m_pTexture, &rect, &rect);
}


void Image::InitTexture()
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 rmask = 0xff000000;
    const Uint32 gmask = 0x00ff0000;
    const Uint32 bmask = 0x0000ff00;
    const Uint32 amask = 0x000000ff;
#else
    const Uint32 rmask = 0x000000ff;
    const Uint32 gmask = 0x0000ff00;
    const Uint32 bmask = 0x00ff0000;
    const Uint32 amask = 0xff000000;
#endif

    if (m_pTexture != nullptr)
        SDL_DestroyTexture(m_pTexture);

    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_width, m_height, 32, rmask, gmask, bmask, amask);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}
