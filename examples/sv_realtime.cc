#include "ns3/realtime-simulator-impl.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include "ns3/sv-subscriber-app.h"

#include <chrono>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SVRealime");

static void onReceive(Ptr<Application> app, int numberOfPublishers, uint64_t oldValue, uint64_t newValue) {
    auto subscriber = DynamicCast<SVSubscriber>(app);
    auto lastSample = subscriber->GetLastSample();
    float timestamp = lastSample.sampleTimestamp / 1000000.0;
    std::cout << numberOfPublishers << ";" << lastSample.sampleTimestamp << ";" << lastSample.receivedTimestamp << std::endl;
}

static void sample(int frequency, int samplesPerCycle, int numberOfPublishers, Time stopTime) {

    auto publisherNodes = NodeContainer(numberOfPublishers);
    auto nodes = NodeContainer(publisherNodes);
    nodes.Create(1);
    auto subscriberNode = nodes.Get(numberOfPublishers);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto publisher = SVPublisherHelper();
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(frequency * samplesPerCycle * stopTime.GetSeconds()));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(frequency));
    publisher.SetAttribute("SamplesPerCycle", ns3::UintegerValue(samplesPerCycle));

    auto publisherApps = publisher.Install(publisherNodes);

    auto subscriber = SVSubscriberHelper();
    auto subscriberApps = subscriber.Install(subscriberNode);

    Simulator::Stop(stopTime);

    auto subscriberApp = subscriberNode->GetApplication(0);
    subscriberApp->TraceConnectWithoutContext(
        "Received",
        MakeBoundCallback(&onReceive, subscriberApp, numberOfPublishers)
        );

    Simulator::Run();

    Simulator::Destroy();
}

int main() {
    constexpr int frequency = 60;
    constexpr int messagesPerCycle = 80;
    const bool realtime = true;

    if (realtime) {
        GlobalValue::Bind("SimulatorImplementationType", StringValue("ns3::RealtimeSimulatorImpl"));
        Config::SetDefault("ns3::RealtimeSimulatorImpl::SynchronizationMode", StringValue("HardLimit"));
    }

    std::cout << "n_publishers;message_timestamp;receive_timestamp" << std::endl;
    for (int i = 0; i < 16; i++)
        sample(frequency, messagesPerCycle, i, Seconds(10));
    return 0;
}
