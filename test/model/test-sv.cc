#include "test-sv.h"

#include "ns3/sv-subscriber-app.h"
#include "ns3/csma-helper.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/sv-helper.h"

#include <cmath>

using namespace ns3;

struct Observed {
    int sent;
    int received;
    double firstSamples[8];
};

static void traceSent(Observed *test, uint64_t oldValue, uint64_t newValue) {
    test->sent = newValue;
}

static void traceReceived(Observed *test, Ptr<Application> app, uint64_t oldValue, uint64_t newValue) {
    const auto subscriber = DynamicCast<SVSubscriber>(app);
    const auto sample = subscriber->GetLastSample();

    if (oldValue < 8) {
        test->firstSamples[oldValue] = sample.ia;
    }

    test->received = newValue;
}

void
TestSVProtection::DoRun()
{
    return;
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

    auto observed = Observed();

    auto publisherApp = clientNode->GetApplication(0);
    publisherApp->TraceConnectWithoutContext(
        "Sent", ns3::MakeBoundCallback(&traceSent, &observed)
        );

    auto subscriberApp = serverNode->GetApplication(0);
    subscriberApp->TraceConnectWithoutContext(
        "Received", ns3::MakeBoundCallback(&traceReceived, &observed, subscriberApp)
        );

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();

    NS_TEST_ASSERT_MSG_EQ(
            observed.sent,
            packetsToSend,
            "Number of sent packets must be " + std::to_string(packetsToSend)
        );

    NS_TEST_ASSERT_MSG_EQ(
        observed.received,
        packetsToSend,
        "Number of received samples must be " + std::to_string(packetsToSend)
    );

    double expectedSamples[] = {0, 1130.23, 2256, 3372.86, 4476.42, 5562.31, 6626.24, 7664.03};

    for (int i = 0; i < 8; i++) {
        auto sample = observed.firstSamples[i];
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

void TestSVMeasurement::DoRun() {
    constexpr int packetsToSend = 1;
    constexpr int frequency = 50;
    constexpr int samplesPerCycle = 100;
    constexpr int samplesPerMessage = 8;

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
    publisher.SetAttribute("SamplesPerMessage", ns3::UintegerValue(samplesPerMessage));
    publisher.Install(clientNode);

    auto subscriber = ns3::SVSubscriberHelper();
    subscriber.Install(serverNode);

    auto observed = Observed();

    auto publisherApp = clientNode->GetApplication(0);
    publisherApp->TraceConnectWithoutContext(
        "Sent", ns3::MakeBoundCallback(&traceSent, &observed)
        );

    auto subscriberApp = serverNode->GetApplication(0);
    subscriberApp->TraceConnectWithoutContext(
        "Received", ns3::MakeBoundCallback(&traceReceived, &observed, subscriberApp)
        );

    ns3::Simulator::Run();
    ns3::Simulator::Destroy();

    NS_TEST_ASSERT_MSG_EQ(
        observed.sent,
        packetsToSend,
        "Number of sent packets must be " + std::to_string(packetsToSend)
    );

    NS_TEST_ASSERT_MSG_EQ(
        observed.received,
        packetsToSend * samplesPerMessage,
        "Number of received samples must be " + std::to_string(packetsToSend * 8)
    );

    double expectedSamples[] = {0, 1130.23, 2256, 3372.86, 4476.42, 5562.31, 6626.24, 7664.03};
    for (int i = 0; i < 8; i++) {
        auto sample = observed.firstSamples[i];
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
