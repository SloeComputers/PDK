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

   bool connected() const;
   bool empty() const override;
   uint8_t rx() override;
   void tx(uint8_t byte) override;

private:
   class Pimpl;

   Pimpl* pimpl{};
};


} // namespace MIDI

} // namespace PLT
