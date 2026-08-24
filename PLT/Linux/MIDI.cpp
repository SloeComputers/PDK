//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <queue>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include "PLT/MIDI.h"

namespace PLT {

namespace MIDI {


class Interface::Pimpl
{
public:
   Pimpl(unsigned device_out_index_)
   {
      // XXX assume that all Linux MIDI devices are called midiN
      //     where N is an integer between 1 and MAX_MIDI_INDEX
      for(unsigned index = 0; index <= MAX_MIDI_INDEX; ++index)
      {
         char device_name[FILENAME_MAX];
         snprintf(device_name, sizeof(device_name), "/dev/midi%u", index);

         int fd = open(device_name, O_RDONLY | O_NONBLOCK);
         if (fd != -1)
         {
            fd_list.push_back(fd);
         }
      }
   }

   ~Pimpl()
   {
      for(const auto& fd : fd_list)
      {
         close(fd);
      }
   }

   bool connected() const { return not fd_list.empty(); }

   bool empty() const { return fifo.empty(); }

   void read()
   {
      for(const auto& fd : fd_list)
      {
         uint8_t buffer[512];

         while(true)
         {
            int status = ::read(fd, buffer, sizeof(buffer));
            if (status <= 0)
               break;

            for(int i = 0; i < status; ++i)
            {
               fifo.push(buffer[i]);
            }
         }
      }
   }

   uint8_t recv()
   {
      uint8_t byte = fifo.front();
      fifo.pop();
      return byte;
   }

   void send(uint8_t byte)
   {
      // TODO
   }

private:
   static const unsigned MAX_MIDI_INDEX = 16;

   std::vector<int>    fd_list;
   std::queue<uint8_t> fifo;
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
   return pimpl->connected();
}

bool Interface::empty() const
{
   pimpl->read();
   return pimpl->empty();
}

uint8_t Interface::rx()
{
   return pimpl->recv();
}

void Interface::tx(uint8_t byte)
{
   return pimpl->send(byte);
}


} // namespace MIDI

} // namespace PLT
