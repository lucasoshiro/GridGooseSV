#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"
#include "ns3/sv-subscriber-app.h"
#include "ns3/command-line.h"
#include "ns3/string.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleSV");

struct Parameters {
    int frequency;
    Time stopTime;
    bool measurement;
};

static void onReceive(Ptr<Application> app, uint64_t oldValue, uint64_t newValue) {
     auto subscriber = DynamicCast<SVSubscriber>(app);
     auto lastSample = subscriber->GetLastSample();
     float timestamp = lastSample.sampleTimestamp / 1000000.0;
     std::cout << timestamp << ";" << lastSample.ia << ";" << lastSample.ib << ";" << lastSample.ic << std::endl;
}

static void sample(Parameters params) {
    auto nodes = NodeContainer(2);
    auto publisherNode = nodes.Get(0);
    auto subscriberNode = nodes.Get(1);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto publisher = SVPublisherHelper();
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(0));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(params.frequency));
    publisher.SetAttribute(
        "SamplesPerCycle",
        ns3::UintegerValue(params.measurement ? 256 : 80)
    );
    publisher.SetAttribute(
        "SamplesPerMessage",
        ns3::UintegerValue(params.measurement ? 8 : 1)
    );

    auto publisherApps = publisher.Install(publisherNode);

    auto subscriber = SVSubscriberHelper();
    auto subscriberApps = subscriber.Install(subscriberNode);

    publisherApps.Stop(params.stopTime);

    auto application = subscriberNode->GetApplication(0);
    application->TraceConnectWithoutContext(
        "Received", MakeBoundCallback(&onReceive, application)
    );

    Simulator::Run();
    Simulator::Destroy();
}

int main(int argc, char *argv[]) {
    CommandLine cmd(__FILE__);

    constexpr int frequency = 60;

    auto mode = std::string("protection");

    cmd.AddValue("mode", "measurement or protection", mode);
    cmd.Parse(argc, argv);

    Parameters params = {
        .frequency = frequency,
        .stopTime = MilliSeconds(100)
    };

    if (mode == "protection") params.measurement = false;
    else if (mode == "measurement") params.measurement = true;
    else NS_ABORT_MSG("Unknown mode");

    std::cout << "time;ia;ib;ic" << std::endl;

    sample(params);

    return 0;
}