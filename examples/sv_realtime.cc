#include "ns3/realtime-simulator-impl.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"
#include "ns3/string.h"
#include "ns3/config.h"
#include "ns3/sv-subscriber-app.h"
#include "ns3/command-line.h"

#include <chrono>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SVRealime");

struct Parameters {
    int frequency;
    Time stopTime;
    int numberOfPublishers;
    bool measurement;
};

static void onReceive(Ptr<Application> app, int numberOfPublishers, uint64_t oldValue, uint64_t newValue) {
    auto subscriber = DynamicCast<SVSubscriber>(app);
    auto lastSample = subscriber->GetLastSample();
    float timestamp = lastSample.sampleTimestamp / 1000000.0;
    std::cout << numberOfPublishers << ";" << lastSample.sampleTimestamp << ";" << lastSample.receivedTimestamp << std::endl;
}

static void sample(Parameters params) {
    auto publisherNodes = NodeContainer(params.numberOfPublishers);
    auto nodes = NodeContainer(publisherNodes);
    nodes.Create(1);
    auto subscriberNode = nodes.Get(params.numberOfPublishers);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto samplesPerCycle = params.measurement ? 256 : 80;
    auto samplesPerMessage = params.measurement ? 8 : 1;

    auto publisher = SVPublisherHelper();
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(0));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(params.frequency));
    publisher.SetAttribute("SamplesPerCycle", ns3::UintegerValue(samplesPerCycle));
    publisher.SetAttribute("SamplesPerMessage", ns3::UintegerValue(samplesPerMessage));

    auto publisherApps = publisher.Install(publisherNodes);

    auto subscriber = SVSubscriberHelper();
    auto subscriberApps = subscriber.Install(subscriberNode);

    Simulator::Stop(params.stopTime);

    auto subscriberApp = subscriberNode->GetApplication(0);
    subscriberApp->TraceConnectWithoutContext(
        "Received",
        MakeBoundCallback(&onReceive, subscriberApp, params.numberOfPublishers)
        );

    Simulator::Run();

    Simulator::Destroy();
}

int main(int argc, char *argv[]) {
    constexpr int frequency = 60;
    const bool realtime = true;

    auto mode = std::string("protection");

    CommandLine cmd(__FILE__);
    cmd.AddValue("mode", "measurement or protection", mode);
    cmd.Parse(argc, argv);

    if (realtime) {
        GlobalValue::Bind("SimulatorImplementationType", StringValue("ns3::RealtimeSimulatorImpl"));
        Config::SetDefault("ns3::RealtimeSimulatorImpl::SynchronizationMode", StringValue("HardLimit"));
    }

    bool measurement;
    if (mode == "protection") measurement = false;
    else if (mode == "measurement") measurement = true;
    else NS_ABORT_MSG("Unknown mode");

    std::cout << "n_publishers;message_timestamp;receive_timestamp" << std::endl;
    for (int i = 1; i < 18; i++) {
        Parameters params = {
            .frequency = frequency,
            .stopTime = Seconds(10),
            .numberOfPublishers = i,
            .measurement = measurement,
        };

        sample(params);
    }

    return 0;
}
