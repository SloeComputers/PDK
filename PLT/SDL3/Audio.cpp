//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SDL3/SDL.h"

#include "PLT/Audio.h"

#include <vector>

namespace PLT {

namespace Audio {

using CallBack = void (*)(void*, SDL_AudioStream*, int, int);

class IOBase::Impl
{
public:
   Impl(IOBase*              io_,
        unsigned             device_,
        const SDL_AudioSpec& spec_,
        CallBack             call_back_)
      : io(io_)
   {
      stream = SDL_OpenAudioDeviceStream(device_, &spec_, call_back_, this);

      if (stream == nullptr)
         SDL_Log("SDL_OpenAudioDevice() failed : %s", SDL_GetError());
   }

   ~Impl()
   {
      if (stream != nullptr)
         SDL_DestroyAudioStream(stream);
   }

   bool isOpen() const { return stream != nullptr; }

   void setEnable(bool enable_)
   {
      if (enable_)
         SDL_ResumeAudioStreamDevice(stream);
      else
         SDL_PauseAudioStreamDevice(stream);
   }
 
   template <typename TYPE>
   void send(size_t bytes_)
   {
      if (bytes_ > 0)
      {
         buffer.clear();
         buffer.resize(bytes_);

         unsigned n      = bytes_ / sizeof(TYPE);
         TYPE*    sample = (TYPE*)buffer.data();

         io->getSamples(sample, n);

         SDL_PutAudioStreamData(stream, buffer.data(), buffer.size());
      }
   }

   template <typename TYPE>
   void recv(size_t bytes_)
   {
      if (bytes_ > 0)
      {
         buffer.clear();
         buffer.resize(bytes_);

         SDL_GetAudioStreamData(stream, buffer.data(), buffer.size());

         unsigned n      = bytes_ / sizeof(TYPE);
         TYPE*    sample = (TYPE*)buffer.data();

         io->setSamples(sample, n);
      }
   }

private:
   SDL_AudioStream*     stream{};
   std::vector<uint8_t> buffer{};
   IOBase*              io{};
};


template <typename TYPE>
static void mixAudioIn(void*            userdata,
                       SDL_AudioStream* stream,
                       int              additional_amount,
                       int              total_amount)
{
   IOBase::Impl* impl = (IOBase::Impl*)userdata;

   impl->recv<TYPE>(additional_amount);
}


template <typename TYPE>
static void mixAudioOut(void*            userdata,
                        SDL_AudioStream* stream,
                        int              additional_amount,
                        int              total_amount)
{
   IOBase::Impl* impl = (IOBase::Impl*)userdata;

   impl->send<TYPE>(additional_amount);
}


IOBase::IOBase(unsigned freq_,
               Format   format_,
               unsigned channels_,
               unsigned samples_,
               bool     input)
   : freq(freq_)
   , format(format_)
   , channels(channels_)
{
   static bool sdl_init{false};
  
   if(not sdl_init)
   {
      SDL_Init(SDL_INIT_AUDIO);
      sdl_init = true;
   }

   (void) handle;
   // samples_ ignored !!!

   SDL_AudioSpec spec;
   CallBack      call_back;

   spec.freq     = freq;
   spec.channels = channels;

   switch(format)
   {
   case Format::SINT8:
      spec.format = SDL_AUDIO_S8;
      call_back   = input ? mixAudioIn<int8_t> : mixAudioOut<int8_t>;
      break;

   case Format::SINT16:
      spec.format = SDL_AUDIO_S16;
      call_back   = input ? mixAudioIn<int16_t> : mixAudioOut<int16_t>;
      break;

   case Format::SINT32:
      spec.format = SDL_AUDIO_S32;
      call_back   = input ? mixAudioIn<int32_t> : mixAudioOut<int32_t>;
      break;

   default:
      return;
   }

   pimpl = new Impl(this,
                    input ? SDL_AUDIO_DEVICE_DEFAULT_RECORDING
                          : SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                    spec,
                    call_back);

   open = pimpl->isOpen();
}

IOBase::~IOBase()
{
   delete pimpl;
}

bool IOBase::setEnable(bool enable_)
{
   if(enable_ != enable)
   {
      enable = enable_;

      pimpl->setEnable(enable);
   }

   return true;
}

} // namespace Audio

} // namespace PLT
