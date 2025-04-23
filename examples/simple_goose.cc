#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/goose-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleGOOSE");

static void onReceive(uint64_t oldValue, uint64_t newValue) {
    std::cout << Simulator::Now().GetMicroSeconds() << std::endl;
}

int main() {
    auto stopTime = Seconds(0.3);
    auto nodes = NodeContainer(2);

    auto publisherNode = nodes.Get(0);
    auto receiverNode = nodes.Get(1);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    auto devices = csmaHelper.Install(nodes);

    auto publisherHelper = GOOSEPublisherHelper();
    publisherHelper.SetAttribute("EventInterval", TimeValue(Seconds(0.1)));
    auto publisherApps = publisherHelper.Install(publisherNode);

    auto receiverHelper = GOOSEReceiverHelper();
    auto receiverApps = receiverHelper.Install(receiverNode);

    auto application = receiverNode->GetApplication(0);
    application->TraceConnectWithoutContext(
        "Received", MakeCallback(onReceive)
    );

    publisherApps.Stop(stopTime);

    csmaHelper.EnablePcapAll("simple-goose");

    Simulator::Run();
    Simulator::Destroy();
}