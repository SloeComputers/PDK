//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstring>

#include "NDL.h"

using namespace NDL;

Model::~Model()
{
   for(auto node : node_list)
      delete node;
}

Node* Model::constructNode()
{
   return new Node();
}

void Model::del(Node* node_)
{
   for(auto& node : node_list)
   {
      if (node == node_)
      {
         node = node_list.back();
         node_list.pop_back();
         delete node_;
         break;
      }
   }
}

Node* Model::findNode(const char* name_) const
{
   for(auto node : node_list)
   {
      const char* label = node->getLabel();
      if ((label != nullptr) && (strcmp(label, name_) == 0))
         return node;
   }

   return nullptr;
}

void Model::writeDot(const char* dir_)
{
   FILE* fp = NDL::openForWrite(this, dir_, "dot");
   if (fp == nullptr)
      return;

   fprintf(fp, "graph %s {\n", getName());

   for(const auto node : getNodes())
   {
      auto port_list = node->getPorts();

      if (port_list.empty()) continue;

      Port*   from     = port_list[0];
      Device* from_dev = from->getDevice();

      for(size_t i = 1; i < port_list.size(); ++i)
      {
         Port*   to = port_list[i];
         Device* to_dev = to->getDevice();

         fprintf(fp, "  %s:%s -- %s:%s\n",
                 from_dev->getName(),
                 from->getName(),
                 to_dev->getName(),
                 to->getName());
      }
   }

   fprintf(fp, "}\n");

   fclose(fp);
}

void Model::writeXML(const char* dir_)
{
   FILE* fp = NDL::openForWrite(this, dir_, "xml");
   if (fp == nullptr)
      return;

   fprintf(fp, "<model name=\"%s\">\n", getName());

   for(const auto device : getDevices())
   {
      fprintf(fp, "  <device type=\"%s\" name=\"%s\" value=\"",
              device->getType(),
              device->getName());
      device->printValue(fp);
      fprintf(fp, "\">\n");

      for(const auto port : device->getPorts())
      {
         fprintf(fp, "    <port name=\"%s\"/>\n", port->getName());
      }

      fprintf(fp, "  </device>\n");
   }

   for(const auto node : getNodes())
   {
      auto port_list = node->getPorts();

      if (port_list.empty()) continue;

      Port*   from     = port_list.front();
      Device* from_dev = from->getDevice();

      for(const auto port : node->getPorts())
      {
         if (port != from)
         {
            Device* to_dev = port->getDevice();
            fprintf(fp, "  <connect from=\"%s.%s\" to=\"%s.%s\"/>\n",
                    from_dev->getName(),
                    from->getName(),
                    to_dev->getName(),
                    port->getName());
         }
      }
   }

   fprintf(fp, "</model>\n");

   fclose(fp);
}

void Model::renumberNodes()
{
   for(unsigned i = 0; i < node_list.size(); ++i)
   {
      node_list[i]->setId(i);
   }
}
