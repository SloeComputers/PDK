//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "NDL.h"

using namespace NDL;

Port::Port(Device* device_, const char* name_)
   : device(device_)
   , name(name_)
{
   device->add(this);
}

Port::~Port()
{
   device->rem(this);

   if (node != nullptr)
   {
      node->rem(this);

      if (node->empty())
         device->del(node);
   }
}

void Port::setLabel(const char* label_)
{
   if (port != nullptr)
   {
      port->setLabel(label_);
      return;
   }

   if (node == nullptr)
   {
      error("Port '%s.%s' has no connections\n",
            device->getName(), name);
      return;
   }

   node->setLabel(label_);
}

void Port::connect(Port& target_)
{
   if (port != nullptr)
   {
      port->connect(target_);
      return;
   }
   else if (target_.port != nullptr)
   {
      target_.connect(*this);
      return;
   }

   Node* target_node = target_.getNode();

   if (node == nullptr)
   {
      if (target_node == nullptr)
      {
         node = device->makeNode();

         target_.node = node;
         node->add(&target_);
      }
      else
      {
         node = target_node;
      }

      node->add(this);
   }
   else if (target_node != node)
   {
      target_.node = node;

      if (target_node == nullptr)
      {
         node->add(&target_);
      }
      else
      {
         for(const auto port : target_node->getPorts())
         {
            node->add(port);
            port->node = node;
         }

         device->del(target_node);
      }
   }
}
