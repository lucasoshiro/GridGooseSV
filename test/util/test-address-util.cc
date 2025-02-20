#include "test-address-util.h"

#include "ns3/csma-helper.h"
#include "ns3/node-container.h"

#include "ns3/address-util.h"

void
TestFormatAddress::DoRun()
{
    uint8_t address[] = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0x06};

    auto formatted = std::string(formatMacAddress(address));
    auto expected = "a1:b2:c3:d4:e5:06";

    NS_TEST_ASSERT_MSG_EQ(formatted, expected, "Expected addresss to be a1:b2:c3:d4:e5:06");
}

void
TestGetMacAddress::DoRun()
{
    uint8_t address[] = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0x06};
    auto mac48addr = getMacAddress(address);

    uint8_t buffer[6];
    mac48addr.CopyTo(buffer);

    auto expected = "a1:b2:c3:d4:e5:06";

    NS_TEST_ASSERT_MSG_EQ(mac48addr, expected, "Expected address to be a1:b2:c3:d4:e5:06");
}

void
TestGetPacketSocketAddress::DoRun()
{
    ns3::NodeContainer nodes;
    nodes.Create(2);

    ns3::CsmaHelper csmaHelper;
    auto devices = csmaHelper.Install(nodes);

    auto destAddress = devices.Get(1)->GetAddress();

    uint8_t mac[6];
    destAddress.CopyTo(mac);

    auto packetAddr = ns3::PacketSocketAddress();
    packetAddr.SetPhysicalAddress(destAddress);
    packetAddr.SetSingleDevice(devices.Get(0)->GetIfIndex());

    auto packetSocketAddr = getPacketSocketAddress(devices.Get(0), mac);
    auto physicalAddress = packetSocketAddr.GetPhysicalAddress();
    auto singleDevice = packetSocketAddr.GetSingleDevice();

    NS_TEST_ASSERT_MSG_EQ(
        physicalAddress,
        destAddress,
        "Expected packetSocket destination address to be same as destAddress"
        );

    NS_TEST_ASSERT_MSG_EQ(
        singleDevice,
        devices.Get(0)->GetIfIndex(),
        "Expected packetSocket device to be same node 0 device"
        );
}

