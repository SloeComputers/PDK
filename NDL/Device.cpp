//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "NDL.h"

using namespace NDL;

Device::Device(Model* model_, const char* name_)
   : model(model_)
   , name(name_)
{
   model->add(this);
}

Device::~Device()
{
   model->rem(this);
}

Node* Device::makeNode()
{
   return model->makeNode();
}

void Device::del(Node* node_)
{
   model->del(node_);
}

Node* Device::getOtherNode(Port* port_) const
{
   NDL::Port* other_port = getOtherPort(port_);
   if (other_port == nullptr)
   {
      return nullptr;
   }

   return other_port->getNode();
}

