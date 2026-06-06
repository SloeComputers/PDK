//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MIDI/Instrument.h"

class Midi
{
public:
   Midi() = default;

   void setDebug(bool debug_)
   {
      debug = debug_;
   }

   void attachInstrument(MIDI::Instrument& instrument_);

   void tick()
   {
   }

private:
   bool debug{false};
};
