#include "test-sv.h"

#include "ns3/sv-subscriber-app.h"
#include "ns3/csma-helper.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/sv-helper.h"

#include <cmath>

static int sent = 0;
static int received = 0;
static double firstSamples[8];


using namespace ns3;

static void traceSent(uint64_t oldValue, uint64_t newValue) {
    sent = newValue;
}

static void traceReceived(Ptr<Application> app, uint64_t oldValue, uint64_t newValue) {
    const auto subscriber = DynamicCast<SVSubscriber>(app);
    const auto sample = subscriber->GetLastSample();

    if (oldValue < 8) {
        firstSamples[oldValue] = sample.ia;
    }

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

    auto subscriberApp = serverNode->GetApplication(0);
    subscriberApp->TraceConnectWithoutContext(
        "Received", ns3::MakeBoundCallback(&traceReceived, subscriberApp)
        );

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();

    NS_TEST_ASSERT_MSG_EQ(sent, packetsToSend, "Number of sent packets must be 40");
    NS_TEST_ASSERT_MSG_EQ(received, packetsToSend, "Number of sent packets must be 40");

    double expectedSamples[] = {0, 1130.23, 2256, 3372.86, 4476.42, 5562.31, 6626.24, 7664.03};

    for (int i = 0; i < 8; i++) {
        auto sample = firstSamples[i];
        auto expectedSample = expectedSamples[i];
        auto msg = std::stringstream();
        msg << "Expected sample " << i << " to be " << expectedSample;

        NS_TEST_ASSERT_MSG_LT(
            abs(sample - expectedSample),
            0.01,
            msg.str().c_str()
            );
    }
}
