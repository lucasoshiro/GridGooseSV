#include "goose-receiver-app.h"
#include <iostream>
#include <fstream>

#include "ns3/uinteger.h"

static void
gooseListener(
    libiec61850::GooseSubscriber subscriber,
    void* parameter
    )
{
    static_cast<ns3::GOOSEReceiver *>(parameter)->Receive(subscriber);
}

ns3::GOOSEReceiver::
GOOSEReceiver()
{
    NS_LOG_FUNCTION(this);
}

ns3::TypeId
ns3::GOOSEReceiver::GetTypeId()
{
    static TypeId tid =TypeId("ns3::GOOSEReceiver")
       .SetParent<Application>()
       .SetGroupName("LibIEC61850")
       .AddConstructor<GOOSEReceiver>()
       .AddAttribute(
           "DeviceIndex",
           "Index of the NetDevice that will be used to send message. 0 by default",
           UintegerValue(0),
           MakeUintegerAccessor(&GOOSEReceiver::deviceIndex),
           MakeUintegerChecker<u_int64_t>()
       )
       .AddTraceSource(
           "Received",
           "Received packets",
           MakeTraceSourceAccessor(&GOOSEReceiver::received),
           "ns3::TracedValueCallback::Uint64"
    );

    return tid;
}

void
ns3::GOOSEReceiver::StartApplication()
{
    NS_LOG_FUNCTION(this);
    auto nodeId = this->GetNode()->GetId();

    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/" + std::to_string(this->deviceIndex);

    this->receiver = libiec61850::GooseReceiver_create();
    libiec61850::GooseReceiver_setInterfaceId(receiver, path.c_str());

    char goCbRef[] = "simpleIOGenericIO/LLN0$GO$gcbAnalogValues";
    this->subscriber = libiec61850::GooseSubscriber_create(
        goCbRef,
        nullptr
        );

    uint8_t dstMac[6] = {0x01,0x0c,0xcd,0x01,0x00,0x01};
    libiec61850::GooseSubscriber_setDstMac(this->subscriber, dstMac);
    libiec61850::GooseSubscriber_setAppId(this->subscriber, 1000);

    libiec61850::GooseSubscriber_setListener(this->subscriber, gooseListener, this);

    this->received = 0;

    libiec61850::GooseReceiver_addSubscriber(this->receiver, subscriber);

    libiec61850::GooseReceiver_start(this->receiver);
}

void
ns3::GOOSEReceiver::StopApplication()
{
    NS_LOG_FUNCTION(this);

    libiec61850::GooseReceiver_stop(receiver);
    libiec61850::GooseReceiver_destroy(receiver);
}

void ns3::GOOSEReceiver::Receive(libiec61850::GooseSubscriber subscriber) {
    this->lastReceivedTime = ns3::MilliSeconds(
        libiec61850::GooseSubscriber_getTimestamp(this->subscriber)
    );
    this->received++;
}