#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"
#include "ns3/sv-subscriber-app.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleSV");

static void onReceive(Ptr<Application> app, uint64_t oldValue, uint64_t newValue) {
     auto subscriber = DynamicCast<SVSubscriber>(app);
     auto lastSample = subscriber->GetLastSample();
     float timestamp = lastSample.sampleTimestamp / 1000000.0;
     std::cout << timestamp << ";" << lastSample.ia << ";" << lastSample.ib << ";" << lastSample.ic << std::endl;
}

static void sample(int frequency, int samplesPerCycle, Time stopTime) {
    auto nodes = NodeContainer(2);
    auto publisherNode = nodes.Get(0);
    auto subscriberNode = nodes.Get(1);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto publisher = SVPublisherHelper();
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(0));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(frequency));
    publisher.SetAttribute("SamplesPerCycle", ns3::UintegerValue(samplesPerCycle));
    auto publisherApps = publisher.Install(publisherNode);

    auto subscriber = SVSubscriberHelper();
    auto subscriberApps = subscriber.Install(subscriberNode);

    publisherApps.Stop(stopTime);

    auto application = subscriberNode->GetApplication(0);
    application->TraceConnectWithoutContext(
        "Received", MakeBoundCallback(&onReceive, application)
    );

    Simulator::Run();
    Simulator::Destroy();
}

int main() {
    constexpr int frequency = 60;
    constexpr int messagesPerCycle = 80;

    std::cout << "time;ia;ib;ic" << std::endl;
    sample(frequency, messagesPerCycle, MilliSeconds(100));

    return 0;
}