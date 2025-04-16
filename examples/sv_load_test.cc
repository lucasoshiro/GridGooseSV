#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"

#include <chrono>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SVLoadTest");

static int sample(int frequency, int samplesPerCycle, int numberOfPublishers, Time stopTime) {
    auto publisherNodes = NodeContainer();
    publisherNodes.Create(numberOfPublishers);

    auto subscriberNodes = NodeContainer();
    subscriberNodes.Create(1);

    auto nodes = NodeContainer(publisherNodes, subscriberNodes);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto publisher = SVPublisherHelper();
    publisher.SetAttribute("MaxPackets", ns3::UintegerValue(0));
    publisher.SetAttribute("Frequency", ns3::UintegerValue(frequency));
    publisher.SetAttribute("SamplesPerCycle", ns3::UintegerValue(samplesPerCycle));
    auto publisherApps = publisher.Install(publisherNodes);

    auto subscriber = SVSubscriberHelper();
    auto subscriberApps = subscriber.Install(subscriberNodes);

    publisherApps.Stop(stopTime);

    auto begin = std::chrono::steady_clock::now();
    Simulator::Run();
    Simulator::Destroy();
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    return elapsed;
}

int main() {
    constexpr int frequency = 60;
    constexpr int messagesPerCycle = 80;

    std::cout << "publishers;simulated_time;real_time" << std::endl;
    std::cout << "ia;ib;ic" << std::endl;

    for (int publishers = 1; publishers <= 50; publishers++) {
        for (int i = 0; i < 1; i++) {
            constexpr int seconds = 1;
            const int realTime = sample(frequency, messagesPerCycle, publishers, Seconds(seconds));
            std::cout << publishers << ";" << seconds * 1000 << ";" << realTime << std::endl;
        }
    }

    return 0;
}