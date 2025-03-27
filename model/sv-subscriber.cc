#include "sv-subscriber.h"
#include "ns3/sv_subscriber.h"
#include "ns3/uinteger.h"

static void svUpdateListener(
    libiec61850::SVSubscriber libiecSubscriber,
    void *ns3Subscriber,
    libiec61850::SVSubscriber_ASDU asdu
    )
{
    // std::ofstream output("/dev/pts/1");
    auto &output = std::cout;
    output << "svUpdateListener called" << std::endl;

    auto subscriber = static_cast<ns3::SVSubscriber*>(ns3Subscriber);

    auto svID = libiec61850::SVSubscriber_ASDU_getSvId(asdu);

    output << "  svID: " << svID << std::endl;
    output << "  smpCnt: " << libiec61850::SVSubscriber_ASDU_getSmpCnt(asdu) << std::endl;
    output << "  confRev: " << libiec61850::SVSubscriber_ASDU_getConfRev(asdu)<< std::endl;
    output << "  DATA[0]: " << libiec61850::SVSubscriber_ASDU_getFLOAT32(asdu, 0) << std::endl;
    output << "  DATA[1]: " << libiec61850::SVSubscriber_ASDU_getFLOAT32(asdu, 4) << std::endl;
    output << std::endl;

    //output.close();
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
       .AddConstructor<SVSubscriber>()
       .AddAttribute(
           "DeviceIndex",
           "Index of the NetDevice that will be used to send message. 0 by default",
           UintegerValue(0),
           MakeUintegerAccessor(&SVSubscriber::deviceIndex),
           MakeUintegerChecker<u_int64_t>()
    );
    return tid;
}

void
ns3::SVSubscriber::StartApplication()
{
    NS_LOG_FUNCTION(this);
    auto nodeId = this->GetNode()->GetId();

    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/" + std::to_string(this->deviceIndex);

    this->receiver = libiec61850::SVReceiver_create();

    libiec61850::SVReceiver_setInterfaceId(receiver, path.c_str());

    //TODO: set appID as a parameter
    this->subscriber = libiec61850::SVSubscriber_create(nullptr, 0x4000);
    libiec61850::SVSubscriber_setListener(subscriber, svUpdateListener, this);

    libiec61850::SVReceiver_addSubscriber(receiver, subscriber);
    libiec61850::SVReceiver_start(receiver);
}

void
ns3::SVSubscriber::StopApplication()
{
    libiec61850::SVReceiver_destroy(receiver);
    NS_LOG_FUNCTION(this);
}