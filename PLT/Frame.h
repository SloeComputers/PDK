//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file
//! \brief Platform abstraction interface for raw frame buffer

#pragma once

#include <cstdint>
#include <cassert>

#include "Image.h"

//! Platform abstraction layer
namespace PLT {

//! Raw frame buffer
class Frame : public Image
{
public:
   // Falgs for Frame creation
   static const uint32_t NO_FLAGS    = 0;
   static const uint32_t RESIZABLE   = 1 << 0; //!< Frame can be resized
   static const uint32_t NO_BORDER   = 1 << 1; //!< Frame has no external border pixels
   static const uint32_t FULL_SCREEN = 1 << 2; //!< Frame should cover the whole screen

   // Hints to request a logical pixel size larger than physical pixel size
   static uint32_t SCALE_X(unsigned n) { return ((n - 1) & 0xF) << 4; }
   static uint32_t SCALE_Y(unsigned n) { return ((n - 1) & 0xF) << 8; }

   //! Base class for generators of pixel data
   class Generator
   {
   public:
      //! Get frame size and video interlace config
      virtual void getConfig(uint16_t& width,
                             uint16_t& height,
                             bool&     interlace,
                             uint8_t&  char_height)
      {
         width       = 0;
         height      = 0;
         interlace   = false;
         char_height = 1;
      }

      //! Get frame width
      virtual uint16_t getWidth()
      {
         return 0;
      }

      //! Synchronize generator to start of field
      virtual void startField(unsigned field) {}

      //! Synchronize generator to start of a line
      virtual void startLine(unsigned char_row, unsigned char_line) {}

      //! Get next 32 1BPP pixels for the current line
      virtual uint32_t getPixelData_1BPP()
      {
         uint32_t pixels_1bpp{0};

         for(unsigned i=0; i<4; ++i)
         {
            uint32_t pixels_4bpp = getPixelData_4BPP();
            pixels_1bpp = (pixels_1bpp << 1) | (pixels_4bpp != 0 ? 1 : 0);
         }

         return pixels_1bpp;
      }

      //! Get next 8 4BPP pixels for the current line
      virtual uint32_t getPixelData_4BPP() { return 0; }

      // XXX deprecated
      virtual void getRawPixels(uint8_t* buffer, unsigned line)
      {
         assert(!"deprecated and no implementation");
      }
   };

   //! Construct a new frame
   //
   //! \param title for window based platforms should be the window title
   //! \param width frame width (pixels)
   //! \param height frame height (pixels)
   //! \param flags bitmask of hints for the underlying platform that implements the frame
   Frame(const char* title, unsigned width_, unsigned height_, uint32_t flags_ = NO_FLAGS);

   ~Frame();

   //! Get a platform specific handle for the frame
   virtual void* getHandle() const override;

   //! Get a platform specific id for the frame
   uint32_t getId() const;

   //! Set the flags to use when creating the first window
   void setFlags(uint32_t flags_);

   //! Change the title, for window based platforms should be the window title
   void setTitle(const char* title);

   //! Hide or make visible a window
   void setVisible(bool visible);

   //! Resize the frame buffer
   //
   //! \param width frame width (pixels)
   //! \param height frame height (pixels)
   void resize(unsigned width_, unsigned height_);

   //! Ensure any changes to the frame buffer are displayed
   void refresh();

   //! Add a pixel generator
   void setGenerator(Generator* generator_);

   //! Translate internal event co-ordinates to pixel co-ordinates
   //  For internal use by the PLT implementation when the Frame
   //  can support high-DPI (OS X) or pixel scaling
   static void internal_transEventXyToPixel(uint16_t& x, uint16_t& y);

   //! Return a pointer to this object
   //! Not generaly necessary but helps mapt GUI::Frame API
   PLT::Frame* getFramePtr() { return this; }

private:
   class Impl;

   Impl* pimpl{nullptr};
};

} // namespace PLT

