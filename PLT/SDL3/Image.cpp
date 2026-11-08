//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "SDL3/SDL.h"

#include "PLT/Image.h"

namespace PLT {

unsigned Image::getPixelBits() { return 32; }

uint32_t Image::getPixel(unsigned x, unsigned y) const
{
   uint32_t* pixels = reinterpret_cast<uint32_t*>(buffer);
   return pixels[x + y * pitch / 4];
}

void Image::clear(STB::Colour rgb)
{
   defaultClear(rgb);
}

void Image::point(STB::Colour rgb, unsigned x, unsigned y)
{
   uint32_t* pixels = reinterpret_cast<uint32_t*>(buffer);
#ifdef PDK_TARGET_Emscripten
   pixels[x + y * pitch / 4] = 0xFF000000 | rgb;
#else
   pixels[x + y * pitch / 4] = rgb;
#endif
}

void Image::span(uint32_t rgb, unsigned x1, unsigned y, unsigned x2)
{
   defaultSpan(rgb, x1, y, x2);
}

void Image::blit(const Image& source,
                 unsigned x, unsigned y,
                 unsigned w, unsigned h,
                 unsigned src_x, unsigned src_y)
{
   SDL_Surface* src = static_cast<SDL_Surface*>(source.getHandle());

   SDL_Rect srcrect;
   srcrect.x = src_x;
   srcrect.y = src_y;
   srcrect.w = w;
   srcrect.h = h;

   SDL_Rect dstrect;
   dstrect.x = x;
   dstrect.y = y;
   dstrect.w = srcrect.w;
   dstrect.h = srcrect.h;

   SDL_BlitSurface(src, &srcrect, static_cast<SDL_Surface*>(getHandle()), &dstrect);
}

void Image::lineBlit(uint8_t pixel_mask, STB::Colour one, STB::Colour zero,
                     unsigned x, unsigned y)
{
   defaultLineBlit(pixel_mask, one, zero, x, y);
}

bool Image::save(const char* name) const
{
   return SDL_SavePNG(static_cast<SDL_Surface*>(getHandle()), name);
}

} // namespace PLT
