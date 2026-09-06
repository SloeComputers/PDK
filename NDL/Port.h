//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace NDL {

class Device;
class Node;

//! A device port
class Port
{
public:
   Port(Device* device_, const char* name_);

   ~Port();

   // Block copying
   Port(const Port&) = delete;
   Port& operator=(const Port&) = delete;

   Device*     getDevice() const { return device; }
   const char* getName() const { return name; }

   Node* getNode() const
   {
      return port != nullptr ? port->getNode() : node;
   }

   void setLabel(const char* label_);

   void link(Port& port_)
   {
      port = &port_;
   }

   void connect(Port& target_);

   Port& operator>>(Port& target_)
   {
      connect(target_);
      return *this;
   }

private:
   Device*     device;
   const char* name;
   Node*       node{};
   Port*       port{};
};

} // namespace NDL
