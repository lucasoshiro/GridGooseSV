//
// Created by lso on 11/02/25.
//

#include "test-device-util.h"

#include "ns3/device-util.h"
#include "ns3/node-list.h"
#include "ns3/csma-helper.h"

void
TestGetDevice::DoRun()
{
    ns3::NodeContainer nodes;
    nodes.Create(2);

    ns3::CsmaHelper csma;
    ns3::NetDeviceContainer devices = csma.Install(nodes);

    auto device0 = ns3::GetNetDevice("/NodeList/0/NetDevice/0");
    auto device1 = ns3::GetNetDevice("/NodeList/1/NetDevice/0");

    auto expectedDevice0 = devices.Get(0);
    auto expectedDevice1 = devices.Get(1);

    NS_TEST_ASSERT_MSG_EQ(device0, expectedDevice0, "Device 0 not corresponding");
    NS_TEST_ASSERT_MSG_EQ(device1, expectedDevice1, "Device 1 not corresponding");
}

void
TestGetNode::DoRun()
{
    ns3::NodeContainer nodes;
    nodes.Create(2);

    auto expectedNode0 = nodes.Get(0);
    auto expectedNode1 = nodes.Get(1);

    auto id0 = expectedNode0->GetId();
    auto id1 = expectedNode1->GetId();

    auto node0 = ns3::GetNode("/NodeList/" + std::to_string(id0));
    auto node1 = ns3::GetNode("/NodeList/" + std::to_string(id1));

    NS_TEST_ASSERT_MSG_EQ(node0, expectedNode0, "Node 0 not corresponding");
    NS_TEST_ASSERT_MSG_EQ(node1, expectedNode1, "Node 1 not corresponding");
}