#include "sv-subscriber-app.h"

#include "../../../src/core/model/simulator.h"
#include "ns3/sv_subscriber.h"
#include "ns3/uinteger.h"

ns3::SVSubscriber::
SVSubscriber()
{
    NS_LOG_FUNCTION(this);
}

static void svUpdateListener(
    libiec61850::SVSubscriber subscriber,
    void *subscriberApp,
    libiec61850::SVSubscriber_ASDU asdu
    ) {
    static_cast<ns3::SVSubscriber *>(subscriberApp)->Receive(asdu);
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
       )
       .AddTraceSource(
           "Received",
           "Received sensed data. Equals the number of messages times the number of ASDUs per message",
           MakeTraceSourceAccessor(&SVSubscriber::received),
           "ns3::TracedValueCallback::Uint64"
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

    this->received = 0;
    this->lastSample = _SVSample();

    libiec61850::SVReceiver_addSubscriber(receiver, subscriber);
    libiec61850::SVReceiver_start(receiver);
}

void
ns3::SVSubscriber::StopApplication()
{
    NS_LOG_FUNCTION(this);
    libiec61850::SVReceiver_destroy(receiver);
}

void
ns3::SVSubscriber::Receive(
    libiec61850::SVSubscriber_ASDU asdu
    )
{
    auto ts = libiec61850::SVSubscriber_ASDU_getTimestamp(asdu, 32);


    this->lastSample.sampleTimestamp = Timestamp_getTimeInNs(&ts);
    this->lastSample.receivedTimestamp = Simulator::Now().GetNanoSeconds();

    this->lastSample.smpCnt = libiec61850::SVSubscriber_ASDU_getSmpCnt(asdu);
    this->lastSample.ia = libiec61850::SVSubscriber_ASDU_getFLOAT32(asdu, 0);
    this->lastSample.ib = libiec61850::SVSubscriber_ASDU_getFLOAT32(asdu, 4);
    this->lastSample.ic = libiec61850::SVSubscriber_ASDU_getFLOAT32(asdu, 8);

    this->received++;
}

ns3::SVSubscriber::_SVSample ns3::SVSubscriber::GetLastSample() const {
    return this->lastSample;
}
