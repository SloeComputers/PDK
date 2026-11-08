//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// SDL2 Bitmap implementation

#include "SDL3/SDL.h"

#include "PLT/Bitmap.h"

namespace PLT {

//! Memory image buffer
class Bitmap::Impl
{
public:
   Impl() = default;

   ~Impl()
   {
      resize(0, 0);
   }

   void resize(unsigned width_, unsigned height_)
   {
      if (surface != nullptr)
      {
         SDL_DestroySurface(surface);
      }

      if ((width_ == 0) || (height_ == 0))
      {
         return;
      }

      surface = SDL_CreateSurface(width_, height_, SDL_PIXELFORMAT_ARGB8888);
   }

   uint8_t* getData(unsigned& width_, unsigned& height_, unsigned& pitch_)
   {
      if(surface == nullptr)
      {
         return nullptr;
      }

      width_  = surface->w;
      height_ = surface->h;
      pitch_  = surface->pitch;

      return (uint8_t*)surface->pixels;
   }

   void* getHandle() const { return surface; }

private:
   SDL_Surface* surface{nullptr};
};


Bitmap::Bitmap(unsigned width_, unsigned height_)
   : Image(width_, height_)
{
   pimpl = new Impl();

   resize(width_, height_);
}

Bitmap::~Bitmap()
{
   delete pimpl;
}

void Bitmap::resize(unsigned width_, unsigned height_)
{
   pimpl->resize(width_, height_);

   buffer = pimpl->getData(width, height, pitch);
}

void* Bitmap::getHandle() const
{
   return pimpl->getHandle();
}

} // namespace PLT
