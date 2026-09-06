//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <vector>

namespace NDL {

class Port;

//! A node in the circuit
class Node
{
friend class Port;

public:
   Node() {}

   virtual ~Node() {}

   unsigned getId() const { return id; }

   const char* getLabel() const { return label; }

   const std::vector<Port*>& getPorts() const { return port_list; }

   bool empty() const { return port_list.empty(); }

   void setId(unsigned id_) { id = id_; }

   virtual void setLabel(const char* label_) { label = label_; }

private:
   //! Add a port to a node
   void add(Port* port_)
   {
      port_list.push_back(port_);
   }

   //! Remove a port from a node
   void rem(Port* port_)
   {
      for(auto& port : port_list)
      {
         if (port == port_)
         {
            port = port_list.back();
            port_list.pop_back();
            break;
         }
      }
   }

   unsigned           id;
   const char*        label{};
   std::vector<Port*> port_list{};
};

} // namespace NDL
