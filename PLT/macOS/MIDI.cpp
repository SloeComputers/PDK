//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <CoreMIDI/CoreMIDI.h>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <vector>

#include "PLT/MIDI.h"
#include "STB/Fifo.h"

namespace PLT {

namespace MIDI {


class Interface::Pimpl
{
public:
   Pimpl(unsigned device_out_index_)
   {
      if (not createClient())
         return;

      src_connected = connectSrc();
      dst_connected = connectDst(device_out_index_);
   }

   ~Pimpl()
   {
      for(const auto& source : source_list)
      {
         MIDIPortDisconnectSource(input_port, source);
      }

      if(input_port)
      {
         MIDIPortDispose(input_port);
      }

      if(output_port)
      {
         MIDIPortDispose(output_port);
      }

      if(client)
      {
         MIDIClientDispose(client);
      }
   }

   void send(unsigned length_, const uint8_t* data_)
   {
      MIDIPacketList packet_list{};

      packet_list.numPackets       = 1;
      packet_list.packet[0].length = length_;
      ::memcpy(packet_list.packet[0].data, data_, length_);

      MIDISend(output_port, destination, &packet_list);
   }

   bool                   src_connected{false};
   bool                   dst_connected{false};
   std::mutex             mutex{};
   STB::Fifo<uint8_t, 16> fifo;

private:
   bool getDestination(unsigned device_index_)
   {
      destination = MIDIGetDestination(device_index_);
      if(destination == 0)
      {
         fprintf(stderr, "ERROR - MIDIGetDestination() failed\n");
         return false;
      }

      return true;
   }

   bool createClient()
   {
      OSStatus status = MIDIClientCreate(CFSTR("PDK_client"),
                                         /* notifyProc */   nullptr,
                                         /* notifyRefCon */ nullptr,
                                         &client);
      if (status != noErr)
      {
         fprintf(stderr, "ERROR - MIDIClientCreate() failed[%d]\n", status);
         return false;
      }

      return true;
   }

   bool createInputPort()
   {
      OSStatus status = MIDIInputPortCreate(client,
                                            CFSTR("PDK_port"),
                                            midiInputCallBack,
                                            this,
                                            &input_port);
      if(status != noErr)
      {
         fprintf(stderr, "ERROR - MIDIInputPortCreate() failed [%d]\n", status);
         return false;
      }

      return true;
   }

   bool createOutputPort()
   {
      OSStatus status = MIDIOutputPortCreate(client, CFSTR("PDK_port"), &output_port);
      if(status != noErr)
      {
         fprintf(stderr, "ERROR - MIDIOutputPortCreate() failed [%d]\n", status);
         return false;
      }

      return true;
   }

   bool connectSrc()
   {
      if(not createInputPort())
         return false;

      for(unsigned i = 0; i < MIDIGetNumberOfSources(); ++i)
      {
          MIDIEndpointRef source = MIDIGetSource(i);
          if(source == 0)
          {
             fprintf(stderr, "ERROR - MIDIGetSource() failed\n");
             return false;
          }

          OSStatus status = MIDIPortConnectSource(input_port, source, /* connRefCon */ nullptr);
          if(status != noErr)
          {
             fprintf(stderr, "ERROR - MIDIConnectSource() failed [%d]\n", status);
             return false;
          }

          source_list.push_back(source);
      }

      return not source_list.empty();
   }

   bool connectDst(unsigned device_index_)
   {
      if(not createOutputPort())
         return false;

      return getDestination(device_index_);
   }

   static void midiInputCallBack(const MIDIPacketList* pkt_list_,
                                 void*                 read_proc_ref_con_,
                                 void*                 src_conn_ref_con_)
   {
      auto pimpl = (Pimpl*)read_proc_ref_con_;

      const MIDIPacket* pkt = pkt_list_->packet;

      for(uint32_t i = 0; i < pkt_list_->numPackets; ++i)
      {
         std::lock_guard<std::mutex> lock{pimpl->mutex};

         for(unsigned i = 0; i < pkt->length; ++i)
         {
            pimpl->fifo.push(pkt->data[i]);
         }

         pkt = MIDIPacketNext(pkt);
      }
   }

   std::vector<MIDIEndpointRef> source_list{};
   MIDIEndpointRef              destination{};
   MIDIClientRef                client{};
   MIDIPortRef                  input_port{};
   MIDIPortRef                  output_port{};
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
