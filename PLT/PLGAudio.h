//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "SIG/SIG.h"

class Audio
{
public:
   class Instrument
   {
   public:
      Instrument() = default;

      virtual SIG::Signal getSample() = 0;
   };

   Audio() = default;

   void attachInstrument(Instrument& instrument_);
};
