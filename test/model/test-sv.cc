#include "test-sv.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-publisher.h"
#include "ns3/ethernet-server.h"

void
TestSV::DoRun()
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

    auto client = ns3::CreateObject<ns3::SVPublisher>();
    client->SetAttribute("MaxPackets", ns3::UintegerValue(10));
    clientNode->AddApplication(client);

    auto server = ns3::CreateObject<ns3::EthernetServer>();
    serverNode->AddApplication(server);

    csmaHelper.EnablePcapAll("test-sv");

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();
}