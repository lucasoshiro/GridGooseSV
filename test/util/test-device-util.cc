//
// Created by lso on 11/02/25.
//

#include "test-device-util.h"

#include "ns3/device-util.h"
#include "ns3/node-container.h"
#include "ns3/csma-helper.h"

void
TestDeviceUtil::DoRun()
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
