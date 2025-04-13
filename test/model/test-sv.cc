#include "test-sv.h"

#include "ns3/csma-helper.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/sv-helper.h"

static int sent = 0;
static int received = 0;

static void traceSent(uint64_t oldValue, uint64_t newValue) {
    sent = newValue;
}

static void traceReceived(uint64_t oldValue, uint64_t newValue) {
    received = newValue;
}

void
TestSV::DoRun()
{
    constexpr int packetsToSend = 40;
    constexpr int frequency = 50;
    constexpr int samplesPerCycle = 100;

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
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(packetsToSend));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(frequency));
    publisher.SetAttribute("SamplesPerCycle", ns3::UintegerValue(samplesPerCycle));
    publisher.Install(clientNode);

    auto subscriber = ns3::SVSubscriberHelper();
    subscriber.Install(serverNode);

    clientNode->GetApplication(0)->TraceConnectWithoutContext(
        "Sent", ns3::MakeCallback(&traceSent)
        );

    serverNode->GetApplication(0)->TraceConnectWithoutContext(
        "Received", ns3::MakeCallback(&traceReceived)
        );

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();

    NS_TEST_ASSERT_MSG_EQ(sent, packetsToSend, "Number of sent packets must be 40");
    NS_TEST_ASSERT_MSG_EQ(received, packetsToSend, "Number of sent packets must be 40");
}