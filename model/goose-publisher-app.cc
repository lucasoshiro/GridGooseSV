#include "goose-publisher-app.h"
#include "ns3/simulator.h"
#include "ns3/goose_publisher.h"
#include "ns3/uinteger.h"

ns3::GOOSEPublisher::
GOOSEPublisher()
{
    NS_LOG_FUNCTION(this);
}

ns3::TypeId
ns3::GOOSEPublisher::GetTypeId()
{
    static TypeId tid =
    TypeId("ns3::GOOSEPublisher")
        .SetParent<Application>()
        .SetGroupName("LibIEC61850")
        .AddConstructor<GOOSEPublisher>()
        .AddAttribute(
            "DeviceIndex",
            "Index of the NetDevice that will be used to send message. 0 by default",
            UintegerValue(0),
            MakeUintegerAccessor(&GOOSEPublisher::deviceIndex),
            MakeUintegerChecker<u_int64_t>()

        )
        .AddAttribute(
            "MaxPackets",
            "Maximum number of packets to send or 0 for inifinite",
             UintegerValue(0),
            MakeUintegerAccessor(&GOOSEPublisher::count),
            MakeUintegerChecker<u_int64_t>()
        )
        .AddAttribute(
            "T0",
            "The T0 time",
            TimeValue(MilliSeconds(20)),
            MakeTimeAccessor(&GOOSEPublisher::t0),
            MakeTimeChecker()
        )
        .AddTraceSource(
            "Sent",
            "Number of sent packages",
            MakeTraceSourceAccessor(&GOOSEPublisher::sent),
            "ns3::TracedValueCallback::Uint64"
        )
    ;

    return tid;
}

void
ns3::GOOSEPublisher::StartApplication()
{
    NS_LOG_FUNCTION(this);
    auto nodeId = this->GetNode()->GetId();

    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/" + std::to_string(this->deviceIndex);

    this->dataSetValues = LinkedList_create();

    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
    LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

    libiec61850::CommParameters gooseCommParameters;

    // TODO: this should be an attribute
    gooseCommParameters.appId = 1000;

    // TODO: this should be an attribute
    gooseCommParameters.dstAddress[0] = 0x01;
    gooseCommParameters.dstAddress[1] = 0x0c;
    gooseCommParameters.dstAddress[2] = 0xcd;
    gooseCommParameters.dstAddress[3] = 0x01;
    gooseCommParameters.dstAddress[4] = 0x00;
    gooseCommParameters.dstAddress[5] = 0x01;
    gooseCommParameters.vlanId = 0;
    gooseCommParameters.vlanPriority = 4;

    this->publisher = GoosePublisher_create(
        &gooseCommParameters,
        path.c_str()
    );

    char goCbRef[] = "simpleIOGenericIO/LLN0$GO$gcbAnalogValues";
    GoosePublisher_setGoCbRef(publisher, goCbRef);
    GoosePublisher_setConfRev(publisher, 1);

    char dataSetRef[] = "simpleIOGenericIO/LLN0$AnalogValues";
    GoosePublisher_setDataSetRef(publisher, dataSetRef);
    GoosePublisher_setTimeAllowedToLive(publisher, 500);

    this->sent = 0;
    if (this->count <= 0) this->count = -1;

    this->eventId = ns3::Simulator::ScheduleNow(
        &GOOSEPublisher::Send,
        this
        );
}

void
ns3::GOOSEPublisher::Send()
{
    if (this->count == 0) return;

    libiec61850::GoosePublisher_publish(
        this->publisher,
        dataSetValues
    );

    this->sent++;
    this->count--;

    this->eventId = ns3::Simulator::Schedule(
        this->t0,
        &GOOSEPublisher::Send,
        this
        );
}

void
ns3::GOOSEPublisher::StopApplication()
{
    NS_LOG_FUNCTION(this);
    Simulator::Cancel(this->eventId);
    libiec61850::GoosePublisher_destroy(this->publisher);
    LinkedList_destroyDeep(
        this->dataSetValues,
        reinterpret_cast<LinkedListValueDeleteFunction>(MmsValue_delete)
        );
}
