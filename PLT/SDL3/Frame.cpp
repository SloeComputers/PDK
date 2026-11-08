//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SDL3/SDL.h"

#include "PLT/Frame.h"

#include <string>


static PLT::Frame::Generator* generator{nullptr};

#ifdef PDK_TARGET_Emscripten
static unsigned high_dpi_scale{1};
#else
static unsigned high_dpi_scale{2};
#endif
static unsigned scale_x{1};
static unsigned scale_y{1};


namespace PLT {

class Frame::Impl
{
public:
   Impl(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
      : title(title_)
      , flags(flags_)
   {
      static bool sdl_init = false;

      if(!sdl_init)
      {
         SDL_Init(SDL_INIT_VIDEO);
         sdl_init = true;
      }

      if((width_ != 0) && (height_ != 0))
      {
         createWindow(width_, height_);
      }
   }

   void createWindow(unsigned width_, unsigned height_)
   {
      Uint32 sdl_flags = 0;

#ifndef PDK_TARGET_Emscripten
      // Always try for high DPI
      //sdl_flags |= SDL_WINDOW_ALLOW_HIGHDPI;

      // High DPI only works with widths that are a multiple of 4
      if ((width_ & 3) != 0)
      {
         width_ = (width_ + 4) & ~3;
      }
#endif

      if(flags & Frame::FULL_SCREEN) sdl_flags |= SDL_WINDOW_FULLSCREEN;
      if(flags & Frame::RESIZABLE)   sdl_flags |= SDL_WINDOW_RESIZABLE;
      if(flags & Frame::NO_BORDER)   sdl_flags |= SDL_WINDOW_BORDERLESS;

      static bool first_window = true;
      if (first_window)
      {
         first_window = false;
         scale_x = ((flags >> 4) & 0xF) + 1;
         scale_y = ((flags >> 8) & 0xF) + 1;
      }

      window = SDL_CreateWindow(title.c_str(),
                                width_  * scale_x / high_dpi_scale,
                                height_ * scale_y / high_dpi_scale,
                                sdl_flags);

      // Check if high DPI hint has been ignored
      int physical_width, physical_height;
      SDL_GetWindowSizeInPixels(window, &physical_width, &physical_height);
      if (unsigned(physical_width) != width_ * scale_x)
      {
         // No high DPI
         high_dpi_scale = 1;
         SDL_SetWindowSize(window, width_, height_);
      }

      renderer = SDL_CreateRenderer(window, "");

      SDL_RaiseWindow(window);

      createSurface(width_, height_);
   }

   ~Impl()
   {
      destroySurface();

#ifndef PDK_TARGET_Emscripten
      SDL_DestroyRenderer(renderer);
#endif

      SDL_DestroyWindow(window);

#ifndef PDK_TARGET_Emscripten
      // TODO is this the right thing to do? what if more than one Frame is in use?
      SDL_Quit();
#endif
   }

   uint8_t* getStorage(unsigned& pitch)
   {
      if(surface == nullptr) return nullptr;

      pitch = surface->pitch;
      return (uint8_t*)surface->pixels;
   }

   void* getHandle() const
   {
      return surface;
   }

   uint32_t getId() const
   {
      return SDL_GetWindowID(window);
   }

   void setTitle(const char* title_)
   {
      title = title_;
      SDL_SetWindowTitle(window, title_);
   }

   void setFlags(uint32_t flags_)
   {
      flags = flags_;
      scale_x = ((flags >> 4) & 0xF) + 1;
      scale_y = ((flags >> 8) & 0xF) + 1;
   }

   void resize(unsigned width_, unsigned height_)
   {
      if(window == nullptr)
      {
         if((width_ != 0) && (height_ != 0))
         {
            createWindow(width_, height_);
         }
         return;
      }

      destroySurface();
      SDL_SetWindowSize(window,
                        width_  * scale_x / high_dpi_scale,
                        height_ * scale_y / high_dpi_scale);
      createSurface(width_, height_);
   }

   void refresh()
   {
      if (texture == nullptr) return;

      SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
      SDL_RenderTexture(renderer, texture, nullptr, nullptr);
      SDL_RenderPresent(renderer);
   }

   void setVisible(bool visible)
   {
      if (visible)
      {
         SDL_ShowWindow(window);
      }
      else
      {
         SDL_HideWindow(window);
      }
   }

private:
   std::string   title;
   uint32_t      flags;
   SDL_Window*   window{nullptr};
   SDL_Renderer* renderer{nullptr};
   SDL_Surface*  surface{nullptr};
   SDL_Texture*  texture{nullptr};

   void createSurface(unsigned width_, unsigned height_)
   {
      surface = SDL_CreateSurface(width_, height_, SDL_PIXELFORMAT_ARGB8888);

// TODO understand what's going on here!
#ifdef PDK_TARGET_Emscripten
      texture = SDL_CreateTextureFromSurface(renderer, surface);
#else
      texture = SDL_CreateTexture(renderer,
                                  SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  width_, height_);
#endif
      if (texture != nullptr)
      {
         SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
      }
   }

   void destroySurface()
   {
      SDL_DestroyTexture(texture);
      SDL_DestroySurface(surface);
   }
};


Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   pimpl  = new Impl(title_, width, height, flags_);
   buffer = pimpl->getStorage(pitch);
}

Frame::~Frame() { delete pimpl; }

void* Frame::getHandle() const
{
   return pimpl->getHandle();
}

uint32_t Frame::getId() const
{
   return pimpl->getId();
}

void Frame::setTitle(const char* title_)
{
   pimpl->setTitle(title_);
}

void Frame::setFlags(uint32_t flags_)
{
   pimpl->setFlags(flags_);
   pimpl->resize(width, height);
   buffer = pimpl->getStorage(pitch);
}

void Frame::resize(unsigned width_, unsigned height_)
{
   if((width == width_) && (height == height_)) return;

   width  = width_;
   height = height_;

   pimpl->resize(width_, height_);
   buffer = pimpl->getStorage(pitch);
}

void Frame::setVisible(bool visible)
{
   pimpl->setVisible(visible);
}

void Frame::refresh()
{
   pimpl->refresh();
}

void Frame::setGenerator(Generator* generator_)
{
   generator = generator_;
}

void Frame::internal_transEventXyToPixel(uint16_t& x, uint16_t& y)
{
   x = x * high_dpi_scale / scale_x;
   y = y * high_dpi_scale / scale_y;
}

} // namespace PLT
