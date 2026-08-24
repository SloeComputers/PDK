//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdint>
#include <cstdio>
#include <mutex>

#include "PLT/MIDI.h"
#include "STB/Fifo.h"

namespace PLT {

namespace MIDI {


class Interface::Pimpl
{
public:
   Pimpl(unsigned device_out_index_) {}

   ~Pimpl() {}

   void send(unsigned length_, const uint8_t* data_) {}

   bool                   src_connected{false};
   std::mutex             mutex{};
   STB::Fifo<uint8_t, 16> fifo;
};


Interface::Interface(unsigned device_out_)
{
   pimpl = new Pimpl(device_out_);
}

Interface::Interface(::MIDI::Instrument& instrument_, unsigned device_out_, bool debug_)
   : ::MIDI::Interface(instrument_, debug_)
{
   pimpl = new Pimpl(device_out_);
}

Interface::~Interface()
{
   delete pimpl;
}

bool Interface::connected() const
{
   return pimpl->src_connected;
}

bool Interface::empty() const
{
   std::lock_guard<std::mutex> lock{pimpl->mutex};

   return pimpl->fifo.empty();
}

uint8_t Interface::rx()
{
   std::lock_guard<std::mutex> lock{pimpl->mutex};

   uint8_t byte = pimpl->fifo.back();
   pimpl->fifo.pop();
   return byte;
}

void Interface::tx(uint8_t byte_)
{
   pimpl->send(1, &byte_);
}


} // namespace MIDI

} // namespace PLT
