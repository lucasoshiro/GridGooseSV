#include "ns3/goose-receiver-app.h"
#include "ns3/goose_receiver.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/goose-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleGOOSE");

static void onReceive(
    const Ptr<ns3::Application> app,
    uint64_t oldValue, uint64_t newValue
    ) {
    auto subscriberApp = DynamicCast<ns3::GOOSEReceiver>(app);
    auto sendTime = subscriberApp->lastReceivedTime.GetMicroSeconds();
    auto receiveTime = Simulator::Now().GetMicroSeconds();
    std::cout << sendTime << ";" << receiveTime << std::endl;
}

int main() {
    auto stopTime = Seconds(0.3);
    auto nodes = NodeContainer(2);

    auto publisherNode = nodes.Get(0);
    auto receiverNode = nodes.Get(1);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    csmaHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(10)));

    auto devices = csmaHelper.Install(nodes);

    auto publisherHelper = GOOSEPublisherHelper();
    publisherHelper.SetAttribute("EventInterval", TimeValue(Seconds(0.1)));
    auto publisherApps = publisherHelper.Install(publisherNode);

    auto receiverHelper = GOOSEReceiverHelper();
    auto receiverApps = receiverHelper.Install(receiverNode);

    auto application = receiverNode->GetApplication(0);
    application->TraceConnectWithoutContext(
        "Received", MakeBoundCallback(onReceive, application)
    );

    publisherApps.Stop(stopTime);

    csmaHelper.EnablePcapAll("simple-goose");

    std::cout << "send_time" << ";" << "receive_time" << std::endl;
    Simulator::Run();
    Simulator::Destroy();
}