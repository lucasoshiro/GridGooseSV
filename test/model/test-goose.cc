//
// Created by lso on 20/03/25.
//

#include "test-goose.h"

#include "ns3/goose-receiver-app.h"

#include "ns3/csma-helper.h"
#include "ns3/goose-helper.h"
#include "ns3/node-container.h"
#include "ns3/packet-socket-helper.h"

static int sent = 0;
static int received = 0;

static void traceSent(uint64_t oldValue, uint64_t newValue) {
    sent = newValue;
}

static void traceReceived(uint64_t oldValue, uint64_t newValue) {
    received = newValue;
}

void
TestGOOSE::DoRun()
{
    constexpr int packetsToSend = 4;
    auto t0 = ns3::TimeValue(ns3::MilliSeconds(200));

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
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(packetsToSend));
    publisher.SetAttribute("T0", t0);
    publisher.Install(clientNode);

    auto server = ns3::CreateObject<ns3::GOOSEReceiver>();
    serverNode->AddApplication(server);

    clientNode->GetApplication(0)->TraceConnectWithoutContext(
        "Sent", ns3::MakeCallback(&traceSent)
        );

    serverNode->GetApplication(0)->TraceConnectWithoutContext(
        "Received", ns3::MakeCallback(&traceReceived)
        );

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();

    NS_TEST_ASSERT_MSG_EQ(sent, packetsToSend, "Number of sent packets must be 4");
    NS_TEST_ASSERT_MSG_EQ(received, packetsToSend, "Number of sent packets must be 4");
}