//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace MIDI {

class Interface;

//! MIDI instrument base class
class Instrument
{
public:
   Instrument(uint8_t num_voices_, uint8_t base_channel_ = 0)
      : num_voices(num_voices_)
      , base_channel(base_channel_)
   {
      for(unsigned i = 0; i < num_voices; ++i)
         setVoiceState(i, FREE);
   }

   void setInterface(Interface* interface_)
   {
      interface = interface_;
   }

   bool isAnyVoiceOn() const { return getOnVoices() != 0; }

   unsigned getOnVoices() const { return on_voices; }

   unsigned getNumVoices() const { return num_voices; }

   //---------------------------------------------------------------------------
   // Override the following for instrument implementation

   //! Mute a voice
   virtual void voiceMute(unsigned voice_) {}

   //! Reset a voice
   virtual void voiceReset(unsigned voice_) {}

   //! Start a voice
   virtual void voiceOn(unsigned voice_, uint8_t note_, uint8_t velocity_) {}

   //! Stop a voice
   virtual void voiceOff(unsigned voice_, uint8_t velocity_) {}

   //! Set voice aftertouch
   virtual void voicePressure(unsigned voice_, uint8_t level_) {}

   //! Voice pitch bend
   virtual void voicePitchBend(unsigned voice_, int16_t value_) {}

   //! Control a voice
   virtual void voiceControl(unsigned voice_, uint8_t control_, uint8_t value_) {}

   //! Program change
   virtual void voiceProgram(unsigned voice_, uint8_t prog_) {}

   // System Exclusive interface
   virtual void sysEx(uint8_t byte_) {}

   //---------------------------------------------------------------------------
   // Incomming MIDI channel message handlers

   void noteOn(uint8_t channel_, uint8_t note_, uint8_t level_)
   {
      if (level_ == 0)
         return noteOff(channel_, note_, 0);

      if (not isValidChannel(channel_)) return;

      signed index = poly ? getVoice(note_)
                          : channel_ - base_channel;

      if ((index >= 0) && (index < num_voices))
      {
         voiceOn(index, note_, level_);
      }
   }

   void noteOff(uint8_t channel_, uint8_t note_, uint8_t velocity_)
   {
      if (not isValidChannel(channel_)) return;

      signed index{};
      if (poly)
      {
         index = findVoice(note_);
         if (index < 0) return;

         setVoiceState(index, GATE_OFF);
      }
      else
      {
         index = channel_ - base_channel;
      }

      if ((index >= 0) && (index < num_voices))
      {
         voiceOff(index, velocity_);
      }
   }

   void notePressure(uint8_t channel_, uint8_t note_, uint8_t level_)
   {
      if (not isValidChannel(channel_)) return;

      signed index = poly ? findVoice(note_)
                          : channel_ - base_channel;

      if ((index >= 0) && (index < num_voices))
      {
         voicePressure(index, level_);
      }
   }

   void controlChange(uint8_t channel_, uint8_t control_, uint8_t value_)
   {
      if (not isValidChannel(channel_)) return;

      switch (control_)
      {
      case 0x78:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceMute(index);
         }
         break;

      case 0x79:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceReset(index);
         }
         break;

      case 0x7A:
         local_control = value_ == 0x7F;
         break;

      case 0x7B:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceOff(index, /* velocity */ 0);
         }
         break;

      case 0x7C: omni = false; return;
      case 0x7D: omni = true;  return;

      case 0x7E:
         poly         = false;
         num_channels = value_ == 0 ? num_voices : value_;
         break;

      case 0x7F:
         poly         = true;
         num_channels = 1;
         break;

      default:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceControl(index, control_, value_);
         }
         break;
      }
   }

   void programChange(uint8_t channel_, uint8_t prog_)
   {
      if (not isValidChannel(channel_)) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voiceMute(index);
         voiceProgram(index, prog_);
      }
   }

   void channelPressure(uint8_t channel_, uint8_t value_)
   {
      if (not isValidChannel(channel_)) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voicePressure(index, value_);
      }
   }

   void channelPitchBend(uint8_t channel_, int16_t value_)
   {
      if (not isValidChannel(channel_)) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voicePitchBend(index, value_);
      }
   }

   // Channel mode messages

   void allSoundsOff(uint8_t channel_)
   {
      controlChange(channel_, 0x78, 0x00);
   }

   void resetAllControllers(uint8_t channel_)
   {
      controlChange(channel_, 0x79, 0x00);
   }

   void localControl(uint8_t channel_, bool on_)
   {
      controlChange(channel_, 0x7A, on_ ? 0x7F : 0x00);
   }

   void allNotesOff(uint8_t channel_)
   {
      controlChange(channel_, 0x7B, 0x00);
   }

   void omniMode(uint8_t channel_, bool on_)
   {
      controlChange(channel_, on_ ? 0x7D : 0x7C, 0x00);
   }

   void monoMode(uint8_t channel_, uint8_t num_channels_ = 0)
   {
      controlChange(channel_, 0x7E, num_channels_);
   }

   void polyMode(uint8_t channel_)
   {
      controlChange(channel_, 0x7F, 0x00);
   }

protected:
   //! Find an unallocated voice
   signed allocVoice()
   {
      for(unsigned voice = 0; voice < num_voices; ++voice)
      {
         if (voice_state[voice] == FREE)
         {
            ++on_voices;
            return voice;
         }
      }

      return -1;
   }

   //! Deallocate the given voice so that it may be reallocated
   void freeVoice(unsigned voice_)
   {
      if (voice_state[voice_] != FREE)
      {
         setVoiceState(voice_, FREE);
         --on_voices;
      }
   }

   Interface* interface{}; //!< Interface attached to this instrument

private:
   using State = uint8_t;
   using Event = uint8_t;

   bool isValidChannel(uint8_t channel_)
   {
      return omni or ((channel_ >= base_channel) and
                      (channel_ < (base_channel + num_channels)));
   }

   void setVoiceState(signed index_, State state_)
   {
      voice_state[index_] = state_;
      voice_event[index_] = current_event++;
   }

   signed findVoice(uint8_t note_)
   {
      for(unsigned voice = 0; voice < num_voices; ++voice)
      {
         if (voice_state[voice] == note_)
            return voice;
      }

      return -1;
   }

   signed getVoice(uint8_t note_)
   {
      signed voice = allocVoice();
      if (voice < 0)
      {
         // Find the oldest GATE_OFF voice
         uint8_t max_age = 0;

         for(unsigned i = 0; i < num_voices; ++i)
         {
            if (voice_state[i] == GATE_OFF)
            {
               uint8_t age = current_event - voice_event[i];
               if (age > max_age)
               {
                  age   = max_age;
                  voice = i;
                  setVoiceState(voice, note_);
               }
            }
         }
      }

      if (voice < 0)
      {
         // Find the oldest ON voice
         uint8_t max_age = 0;

         for(unsigned i = 0; i < num_voices; ++i)
         {
            uint8_t age = current_event - voice_event[i];
            if (age > max_age)
            {
               age   = max_age;
               voice = i;
            }
         }
      }

      if (voice >= 0)
         setVoiceState(voice, note_);

      return voice;
   }

   static const unsigned MAX_VOICES = 16;

   uint8_t num_voices;
   uint8_t base_channel{0};
   uint8_t num_channels{1};
   bool    local_control{true};
   bool    omni{true};
   bool    poly{true};

   static const State GATE_OFF = 0xFF;
   static const State FREE     = 0xFE;

   Event   current_event{1};
   State   voice_state[MAX_VOICES] = {};  //!< Gate on note value or FREE or GATE_OFF
   Event   voice_event[MAX_VOICES] = {};
   uint8_t on_voices{0};
};

} // namespace MIDI
