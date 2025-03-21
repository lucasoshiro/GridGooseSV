//
// Created by lso on 20/03/25.
//

#include "test-goose.h"

#include "ns3/goose-receiver.h"

#include "ns3/csma-helper.h"
#include "ns3/ethernet-server.h"
#include "ns3/goose-helper.h"
#include "ns3/node-container.h"
#include "ns3/packet-socket-helper.h"

void
TestGOOSE::DoRun()
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

    auto publisher = ns3::GOOSEPublisherHelper();
    //publisher.SetAttribute("MaxPackets", ns3::UintegerValue(10));
    publisher.Install(clientNode);

    auto server = ns3::CreateObject<ns3::GOOSEReceiver>();
    serverNode->AddApplication(server);

    csmaHelper.EnablePcapAll("GOOSE");

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();

}