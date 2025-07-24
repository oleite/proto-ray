//
// Created by Gabriel Leite on 09/07/25.
//

#include "Image.h"





void Image::display() const
{
    SDL_UpdateTexture(m_pTexture, nullptr, m_pixels.data(), width * sizeof(Uint32));

    const SDL_Rect rect = {0, 0, width, height};
    SDL_RenderCopy(m_pRenderer, m_pTexture, &rect, &rect);
}


void Image::initTexture()
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

    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}
