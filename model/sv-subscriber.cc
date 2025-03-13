#include "sv-subscriber.h"

#include "ns3/sv_subscriber.h"

#include <fstream>

static void
svUpdateListener(
    libiec61850::SVSubscriber subscriber,
    void* parameter,
    libiec61850::SVSubscriber_ASDU asdu
    )
{
    std::ofstream output("/dev/pts/1");
    output << "YEYEYEYEYEYEYEYE" << std::endl;
    output.close();
}

ns3::SVSubscriber::
SVSubscriber()
{
    NS_LOG_FUNCTION(this);
}

ns3::TypeId
ns3::SVSubscriber::GetTypeId()
{
    static TypeId tid =TypeId("ns3::SVSubscriber")
       .SetParent<Application>()
       .SetGroupName("LibIEC61850")
       .AddConstructor<SVSubscriber>();
    return tid;
}

void
ns3::SVSubscriber::StartApplication()
{
    NS_LOG_FUNCTION(this);
    auto nodeId = this->GetNode()->GetId();

    //TODO: allow the user change the net device
    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/0";

    this->receiver = libiec61850::SVReceiver_create();

    libiec61850::SVReceiver_setInterfaceId(receiver, path.c_str());

    //TODO: set appID as a parameter
    this->subscriber = libiec61850::SVSubscriber_create(nullptr, 0x4000);
//    libiec61850::SVSubscriber_setListener(subscriber, svUpdateListener, nullptr);

    libiec61850::SVReceiver_addSubscriber(receiver, subscriber);
    //libiec61850::SVReceiver_start(receiver);
}

void
ns3::SVSubscriber::StopApplication()
{
    libiec61850::SVReceiver_destroy(receiver);
    NS_LOG_FUNCTION(this);
}