//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub MIDI implementation

#include "PLT/MIDI.h"

namespace PLT {

namespace MIDI {

Interface::Interface(unsigned device_out_)
{
}

Interface::Interface(::MIDI::Instrument& instrument_, unsigned device_out_, bool debug_)
   : ::MIDI::Interface(instrument_, debug_)
{
}

Interface::~Interface() {}

bool Interface::connected() const { return false; }

bool Interface::empty() const { return true; }

uint8_t Interface::rx() { return 0; }

void Interface::tx(uint8_t byte) {}


} // namespace MIDI

} // namespace PLT
