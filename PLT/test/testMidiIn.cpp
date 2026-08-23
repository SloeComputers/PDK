//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "PLT/MIDI.h"


int main()
{
   MIDI::Instrument     inst{/* num_voices */ 1};
   PLT::MIDI::Interface midi{inst, /* out_device index */ 0, /* debug */ true};

   while(true)
   {
      midi.tick();
   }
}
