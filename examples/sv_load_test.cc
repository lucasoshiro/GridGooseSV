#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"
#include "ns3/command-line.h"

#include <chrono>

using namespace ns3;

struct Parameters {
    int frequency;
    int numberOfPublishers;
    Time stopTime;
    bool measurement;
    bool pcap;
};

NS_LOG_COMPONENT_DEFINE("SVLoadTest");

static int sample(Parameters params) {
    auto publisherNodes = NodeContainer();
    publisherNodes.Create(params.numberOfPublishers);

    auto subscriberNodes = NodeContainer();
    subscriberNodes.Create(1);

    auto nodes = NodeContainer(publisherNodes, subscriberNodes);

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
    auto subscriberApps = subscriber.Install(subscriberNodes);

    publisherApps.Stop(params.stopTime);

    if (params.pcap) {
        csmaHelper.EnablePcapAll("sv_load_test");
    }

    auto begin = std::chrono::steady_clock::now();
    Simulator::Run();
    Simulator::Destroy();
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    return elapsed;
}

int main(int argc, char *argv[]) {
    CommandLine cmd(__FILE__);
    constexpr int frequency = 60;

    auto mode = std::string("protection");
    auto pcap = false;

    cmd.AddValue("mode", "measurement or protection", mode);
    cmd.AddValue("pcap", "generate pcaps", pcap);
    cmd.Parse(argc, argv);

    bool measurement;
    if (mode == "protection") measurement = false;
    else if (mode == "measurement") measurement = true;
    else NS_ABORT_MSG("Unknown mode");

    std::cout << "timestamp;publishers;simulated_time;real_time" << std::endl;

    for (int publishers = 1; publishers <= 256; publishers *= 2) {
        constexpr int seconds = 1;

        double now = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count() / 1.e6;

        Parameters params = {
            .frequency = frequency,
            .numberOfPublishers = publishers,
            .stopTime = Seconds(seconds),
            .measurement = measurement,
            .pcap = pcap
        };

        const int realTime = sample(params);
        std::cout << std::format("{}", now) << ";" << publishers << ";" << seconds * 1000 << ";" << realTime << std::endl;
    }

     return 0;
}