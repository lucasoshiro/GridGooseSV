#include "test-ethernet.h"

#include "../../util/address-util.h"

#include "ns3/core-module.h"
#include "ns3/csma-helper.h"
#include "ns3/ethernet-client.h"
#include "ns3/ethernet-server.h"
#include "ns3/packet-socket-helper.h"

void
TestEthernet::DoRun()
{
    auto nodes = ns3::NodeContainer();
    nodes.Create(2);

    auto clientNode = nodes.Get(0);
    auto serverNode = nodes.Get(1);

    auto packetSocketHelper = ns3::PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = ns3::CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto clientDevice = devices.Get(0);
    auto serverDevice = devices.Get(1);

    auto packetSocketAddr = ns3::PacketSocketAddress();
    packetSocketAddr.SetSingleDevice(clientDevice->GetIfIndex());
    packetSocketAddr.SetPhysicalAddress(serverDevice->GetAddress());

    auto client = ns3::CreateObject<ns3::EthernetClient>();
    client->SetServerAddress(packetSocketAddr);
    clientNode->AddApplication(client);

    auto server = ns3::CreateObject<ns3::EthernetServer>();
    serverNode->AddApplication(server);

    csmaHelper.EnablePcapAll("test-ethernet");

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();
}

void
TestGetInterfaceMACAddress::DoRun()
{
    auto nodes = ns3::NodeContainer();
    nodes.Create(2);

    auto packetSocketHelper = ns3::PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = ns3::CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    for (int i = 0; i < 2; i++)
    {
        auto path = (
            "/NodeList/" +
            std::to_string(devices.Get(i)->GetNode()->GetId()) +
            "/DeviceList/0"
            );

        auto expected = "00:00:00:00:00:0" + std::to_string(i + 1);

        uint8_t macAddress[6];
        libiec61850::Ethernet_getInterfaceMACAddress(path.c_str(), macAddress);

        auto formatted = formatMacAddress(macAddress);
        auto observed = std::string(formatted);
        delete[] formatted;

        NS_TEST_ASSERT_MSG_EQ(observed, expected, "Expected mac address '" + expected + "'");
    }
}