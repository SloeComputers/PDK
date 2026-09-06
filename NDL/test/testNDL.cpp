//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "NDL/NDL.h"

#include "STB/Test.h"

TEST(NDL, basic)
{
   NDL::Model  model{"model"};
   NDL::Device device{&model, "device"};
   NDL::Port   port{&device, "port"};
   NDL::Port   other_port{&device, "other_port"};

   EXPECT_EQ(port.getDevice(), &device);
   EXPECT_EQ(strcmp(port.getName(), "port"), 0);
   EXPECT_EQ(port.getNode(), (NDL::Node*)nullptr);

   port.connect(other_port);
   EXPECT_NE(port.getNode(), (NDL::Node*)nullptr);
   EXPECT_EQ(port.getNode()->getPorts().size(), 2);

   EXPECT_EQ(port.getNode()->getLabel(), (char*)nullptr);
   port.label("label");
   EXPECT_EQ(strcmp(other_port.getNode()->getLabel(), "label"), 0);
   EXPECT_EQ(other_port.getNode(), port.getNode());
}

TEST(NDL, merge)
{
   NDL::Model  model{"model"};
   NDL::Device device{&model, "device"};
   NDL::Port   port_a{&device, "a"};
   NDL::Port   port_b{&device, "b"};
   NDL::Port   port_c{&device, "c"};
   NDL::Port   port_d{&device, "d"};

   port_a.connect(port_b);
   port_c.connect(port_d);
   EXPECT_EQ(model.getNodes().size(), 2);

   port_c.connect(port_a);
   EXPECT_EQ(model.getNodes().size(), 1);
   EXPECT_EQ(port_a.getNode(), port_b.getNode());
   EXPECT_EQ(port_b.getNode(), port_c.getNode());
   EXPECT_EQ(port_c.getNode(), port_d.getNode());
   EXPECT_EQ(port_a.getNode()->getPorts().size(), 4);

   port_a.connect(port_b);
   EXPECT_EQ(model.getNodes().size(), 1);
   EXPECT_EQ(port_a.getNode()->getPorts().size(), 4);
}

TEST(NDL, connectToExistingNode)
{
   NDL::Model  model{"model"};
   NDL::Device device{&model, "device"};
   NDL::Port   port_a{&device, "a"};
   NDL::Port   port_b{&device, "b"};
   NDL::Port   port_c{&device, "c"};
   NDL::Port   port_d{&device, "d"};

   port_a.connect(port_b);

   // Unconnected source joins an existing target node.
   port_c.connect(port_a);
   EXPECT_EQ(port_c.getNode(), port_a.getNode());
   EXPECT_EQ(port_a.getNode()->getPorts().size(), 3);

   // An existing source node accepts an unconnected target.
   port_b.connect(port_d);
   EXPECT_EQ(port_d.getNode(), port_a.getNode());
   EXPECT_EQ(port_a.getNode()->getPorts().size(), 4);
}

TEST(NDL, connectOperator)
{
   NDL::Model  model{"model"};
   NDL::Device device{&model, "device"};
   NDL::Port   port_a{&device, "a"};
   NDL::Port   port_b{&device, "b"};
   NDL::Port   port_c{&device, "c"};

   port_a >> port_b >> port_c;

   EXPECT_EQ(model.getNodes().size(), 1);
   EXPECT_EQ(port_a.getNode(), port_b.getNode());
   EXPECT_EQ(port_b.getNode(), port_c.getNode());
}

TEST(NDL, labels)
{
   NDL::Model  model{"model"};
   NDL::Device device{&model, "device"};
   NDL::Port   port_a{&device, "a"};
   NDL::Port   port_b{&device, "b"};

   port_a.connect(port_b);

   port_a.label("first");
   EXPECT_EQ(model.findNode("first"), port_a.getNode());
   EXPECT_EQ(model.findNode("missing"), (NDL::Node*)nullptr);

   // A later label on the same node is reported as a conflict and takes
   // precedence for lookup.
   port_b.label("second");
   EXPECT_EQ(model.findNode("first"), (NDL::Node*)nullptr);
   EXPECT_EQ(model.findNode("second"), port_a.getNode());
}

TEST(NDL, deviceRegistration)
{
   NDL::Model model{"model"};
   EXPECT_EQ(model.getDevices().size(), 0);

   NDL::Device* device = new NDL::Device(&model, "device");
   EXPECT_EQ(model.getDevices().size(), 1);
   EXPECT_EQ(model.getDevices().front(), device);

   delete device;
   EXPECT_EQ(model.getDevices().size(), 0);
}

TEST(NDL, destruction)
{
   NDL::Model  model{"model"};
   NDL::Device device{&model, "device"};
   NDL::Port*  port_a = new NDL::Port(&device, "a");
   NDL::Port*  port_b = new NDL::Port(&device, "b");

   port_a->connect(*port_b);
   EXPECT_EQ(model.getNodes().size(), 1);

   delete port_a;
   EXPECT_EQ(device.getPorts().size(), 1);
   EXPECT_EQ(model.getNodes().size(), 1);
   EXPECT_EQ(model.getNodes().front()->getPorts().size(), 1);

   delete port_b;
   EXPECT_EQ(device.getPorts().size(), 0);
   EXPECT_EQ(model.getNodes().size(), 0);
}
