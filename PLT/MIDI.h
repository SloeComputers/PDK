//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file
//! \brief Platform abstraction layer for access to MIDI devices

#pragma once

#include <cstdint>

#include "MIDI/Interface.h"


//! Platform abstraction layer
namespace PLT {

//! Access to MIDI devices
namespace MIDI {


class Interface : public ::MIDI::Interface
{
public:
   Interface(unsigned device_out_ = 0);

   Interface(::MIDI::Instrument& instrument_, unsigned device_out_ = 0, bool debug_ = false);

   ~Interface();

   //! Are any input sources connected
   bool connected() const;

   //! Is input FIFO empty
   bool empty() const override;

   //! Get next byte
   uint8_t rx() override;

   //! Send a byte
   void tx(uint8_t byte) override;

private:
   class Pimpl;

   Pimpl* pimpl{};
};


} // namespace MIDI

} // namespace PLT
