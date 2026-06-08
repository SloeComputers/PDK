//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

// XXX undo some crazy namespace corruption by the VST3 build environment
#undef RELEASE

#include "MIDI/Instrument.h"
#include "SIG/SIG.h"

namespace PLT {

class Synth : public MIDI::Instrument
{
public:
   Synth(unsigned num_voices_)
      : MIDI::Instrument(num_voices_)
   {
   }

   virtual ~Synth() = default;

   virtual SIG::Signal getSample() = 0;

   static Synth* construct();
};

} // namespace PLT
