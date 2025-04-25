#include "ns3/goose-receiver-app.h"
#include "ns3/goose_receiver.h"
#include "ns3/packet-socket-helper.h"
#include "ns3/csma-helper.h"
#include "ns3/goose-helper.h"
#include "ns3/command-line.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleGOOSE");

static void onReceive(
    const Ptr<ns3::Application> app,
    uint64_t oldValue, uint64_t newValue
    ) {
    auto subscriberApp = DynamicCast<ns3::GOOSEReceiver>(app);
    auto stNum = subscriberApp->lastStNum;
    auto receiveTime = Simulator::Now().GetMicroSeconds();
    std::cout << stNum << ";" << receiveTime << std::endl;
}

int main(int argc, char* argv[]) {
    CommandLine cmd(__FILE__);

    auto stopTime = Seconds(0.3);
    auto delay = MilliSeconds(10);
    auto nodes = NodeContainer(2);

    cmd.AddValue("stop-time", "Total time of the simulation", stopTime);
    cmd.AddValue("delay", "Channel delay", delay);

    cmd.Parse(argc, argv);

    auto publisherNode = nodes.Get(0);
    auto receiverNode = nodes.Get(1);

    auto packetSocketHelper = PacketSocketHelper();
    packetSocketHelper.Install(nodes);

    auto csmaHelper = CsmaHelper();
    csmaHelper.SetChannelAttribute("Delay", TimeValue(delay));

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

    std::cout << "st_num" << ";" << "receive_time" << std::endl;
    Simulator::Run();
    Simulator::Destroy();
}