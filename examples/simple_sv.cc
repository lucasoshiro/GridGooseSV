#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/sv-helper.h"
#include "ns3/global-value.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleSV");

static struct {
    double a;
    double b;
    double c;
} current;

static void getIA(double oldValue, double newValue) {
    current.a = newValue;
}

static void getIB(double oldValue, double newValue) {
    current.b = newValue;
}

static void getIC(double oldValue, double newValue) {
    float now = Simulator::Now().GetMicroSeconds() / 1000.0;
    current.c = newValue;
    std::cout << now << ";" << current.a << ";" << current.b << ";" << current.c << std::endl;
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

    subscriberNode->GetApplication(0)->TraceConnectWithoutContext("IA", MakeCallback(&getIA));
    subscriberNode->GetApplication(0)->TraceConnectWithoutContext("IB", MakeCallback(&getIB));
    subscriberNode->GetApplication(0)->TraceConnectWithoutContext("IC", MakeCallback(&getIC));

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