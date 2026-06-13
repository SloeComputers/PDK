//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

// XXX undo some crazy namespace corruption by the VST3 build environment
#undef RELEASE

#include "MIDI/Instrument.h"
#include "SIG/SIG.h"

namespace MIDI {

class Synth : public Instrument
{
public:
   Synth(unsigned num_voices_)
      : Instrument(num_voices_)
   {
   }

   virtual ~Synth() = default;

   virtual void setSampleRate(unsigned sample_rate_) {}

   virtual SIG::Signal sample() { return 0; }

   static Synth* construct();
};

} // namespace PLT
