//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <vector>

namespace NDL {

class Model;
class Node;
class Port;

//! Base class for a device
class Device
{
friend class Port;

public:
   Device(Model* model_, const char* name_);

   Device(const Device&) = delete;
   Device& operator=(const Device&) = delete;

   virtual ~Device();

   const char* getName() const { return name; }

   const std::vector<Port*>& getPorts() const { return port_list; }

   virtual const char* getType() const { return "none"; }

   virtual void printValue(FILE* fp_) const {}

   //! For the given port return the other port for a 2 port device
   Port* getOtherPort(Port* port_) const
   {
      if (port_list.size() != 2)
         return nullptr;

           if (port_list[0] == port_) return port_list[1];
      else if (port_list[1] == port_) return port_list[0];
      else                            return nullptr;
   }

   //! For the given port return the node the other port is connected
   //! to for a 2 port device
   Node* getOtherNode(Port* port_) const;

   unsigned id{0};

private:
   Node* makeNode();

   //! Register a port
   void add(Port* port_)
   {
      port_list.push_back(port_);
   }

   //! Un-register a port
   void rem(Port* port_)
   {
      for(auto& port : port_list)
      {
         if (port_ == port)
         {
            port = port_list.back();
            port_list.pop_back();
            break;
         }
      }
   }

   //! Destroy a node
   void del(Node* node_);

   Model*             model;
   const char*        name;
   std::vector<Port*> port_list{};
};

} // namespace NDL
