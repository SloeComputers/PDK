//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file Audio.h
//! \brief Platform abstraction interface for raw audio hardware

#pragma once

#include <cstdint>

//! Platform abstraction layer
namespace PLT {

//! Interface for raw audio hardware
namespace Audio {


//! Audio sample format
enum class Format
{
   NONE = 0, //!< Sample format not configured
   SINT8,    //!< Signed 8-bit sample format
   SINT16,   //!< Signed 16-bit sample format
   SINT32    //!< Signed 32-bit sample format
};


//! Base class for raw audio hardware interfaces
class IOBase
{
public:
   //! Get sample frequency (Hz)
   unsigned getFreq() const { return freq; }

   //! Get sample format
   Format getFormat() const { return format; }

   //! Get number of channels
   //
   //! \return 1 is mono, 2 is stereo
   unsigned getChannels() const { return channels; }

   //! Check if audio interface is ready
   bool isOpen() const { return open; }

   //! Audio interface is enabled
   bool isEnabled() const { return enable; }

   //! Enable or disable audio interface
   //
   //! \param enable true => enable, false => disable
   bool setEnable(bool enable);

   void start() { setEnable(true); }

protected:
   //! Configure and open a connection to a raw audio device
   //
   //! \param freq Sample frequency (Hz)
   //! \param format Sample format
   //! \param channels Number of audio channels (1 or 2)
   //! \param input true => input, false => output
   //! \param sample Number of samples per call-back
   IOBase(unsigned freq, Format format, unsigned channels, unsigned samples, bool input);

   //! Close a connection to a raw audio device
   ~IOBase();

private:
   unsigned  freq{0};
   Format    format{Format::NONE};
   unsigned  channels{0};
   bool      open{false};
   bool      enable{false};
   uintptr_t handle{0};
};


//! Interface to raw audio output hardware
//
//! To use, inherit from this class and override at least one getSamples()
//! method
class Out : public IOBase
{
public:
   //! Configure and open a connection to a raw audio output device
   //
   //! The connections is not enabled by default but can be enabled
   //! using setEnable()
   //! \param freq Sample frequency (Hz)
   //! \param format Sample format
   //! \param channels Number of audio channels (1 or 2)
   //! \param sample Number of samples per call-back
   Out(unsigned freq, Format format, unsigned channels, unsigned samples = 1024)
      : IOBase(freq, format, channels, samples, /* input */ false)
   {}

   virtual ~Out() {}

   //! Overide this method to supply 8-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 8-bit signed (SINT8) sample format. This implementation
   //! fills the buffer with silence.
   //! \param buffer Pointer to a buffer for the audio samples
   //! \param n Number of samples required
   virtual void getSamples(int8_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i] = 0;
   }

   //! Overide this method to supply 16-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 16-bit signed (SINT16) sample format. This implementation
   //! fills the buffer with silence.
   //! \param buffer Pointer to a buffer for the audio samples
   //! \param n Number of samples required
   virtual void getSamples(int16_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i] = 0;
   }

   //! Overide this method to supply 32-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 32-bit signed (SINT32) sample format. This implementation
   //! fills the buffer with silence.
   //! \param buffer Pointer to a buffer for the audio samples
   //! \param n Number of samples required
   virtual void getSamples(int32_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i] = 0;
   }
};


//! Interface to raw audio input hardware
//
//! To use, inherit from this class and override at least one setSamples()
//! method
class In : public IOBase
{
public:
   //! Configure and open a connection to a raw audio input device
   //
   //! The connections is not enabled by default but can be enabled
   //! using setEnable()
   //! \param freq Sample frequency (Hz)
   //! \param format Sample format
   //! \param channels Number of audio channels (1 or 2)
   //! \param sample Number of samples per call-back
   In(unsigned freq, Format format, unsigned channels, unsigned samples = 1024)
      : IOBase(freq, format, channels, samples, /* input */ true)
   {}

   virtual ~In() {}

   //! Overide this method to recieve 8-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 8-bit signed (SINT8) sample format.
   //! \param buffer Pointer to a buffer of audio samples
   //! \param n Number of samples in the buffer
   virtual void setSamples(const int8_t* buffer, unsigned n) {}

   //! Overide this method to recieve 16-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 16-bit signed (SINT16) sample format.
   //! \param buffer Pointer to a buffer of audio samples
   //! \param n Number of samples in the buffer
   virtual void setSamples(const int16_t* buffer, unsigned n) {}

   //! Overide this method to recieve 32-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 32-bit signed (SINT32) sample format.
   //! \param buffer Pointer to a buffer of audio samples
   //! \param n Number of samples in the buffer
   virtual void setSamples(const int32_t* buffer, unsigned n) {}
};


} // namespace Audio

} // namespace PLT

