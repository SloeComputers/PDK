//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <vector>

namespace NDL {

class Node;
class Device;

//! Base class for a simulation
class Model
{
public:
   Model(const char* name_)
      : name(name_)
   {
   }

   Model(const Model&) = delete;
   Model& operator=(const Model&) = delete;

   virtual ~Model();

   //! Return model name
   const char* getName() const { return name; }

   //! Return list of devices
   const std::vector<Device*>& getDevices() const { return device_list; }

   //! Return list of connection nodes
   const std::vector<Node*>& getNodes() const { return node_list; }

   //! Find a node with the given name
   Node* findNode(const char* name_) const;

   //! Write model as a .dot file to the given directory
   void writeDot(const char* dir_);

   //! Write model as a .xml file to the given directory
   void writeXML(const char* dir_);

   //! Assign a contiguous zero based range of ids to all the nodes
   void renumberNodes();

protected:
   //! Construct a new node
   virtual Node* constructNode();

private:
   friend class Device;

   //! Dynamic construction of a connection node
   Node* makeNode()
   {
      Node* node = constructNode();
      node_list.push_back(node);
      return node;
   }

   //! Register a device in the model
   void add(Device* device_)
   {
      device_list.push_back(device_);
   }

   //! Unregister a device from the model
   void rem(Device* device_)
   {
      for(auto& device : device_list)
      {
         if (device == device_)
         {
            device = device_list.back();
            device_list.pop_back();
            break;
         }
      }
   }

   //! Dynamic destruction of a connection node
   void del(Node* node_);

   const char*          name;
   std::vector<Device*> device_list{};
   std::vector<Node*>   node_list{};
};

} // namespace NDL
