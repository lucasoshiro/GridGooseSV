#include "test-sv.h"

#include "ns3/csma-helper.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/sv-helper.h"

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

    auto publisher = ns3::SVPublisherHelper();
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(40));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(50));
    publisher.SetAttribute("SamplesPerCycle", ns3::UintegerValue(100));
    publisher.Install(clientNode);

    auto subscriber = ns3::SVSubscriberHelper();
    subscriber.Install(serverNode);

    csmaHelper.EnablePcapAll("test-sv");

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();
}