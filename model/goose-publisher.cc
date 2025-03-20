#include "goose-publisher.h"
#include "ns3/simulator.h"
#include "ns3/goose_publisher.h"

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
        .AddConstructor<GOOSEPublisher>();

    return tid;
}

void
ns3::GOOSEPublisher::StartApplication()
{
    NS_LOG_FUNCTION(this);
    auto nodeId = this->GetNode()->GetId();

    // TODO: we should make this flexible for cases where the node has more than one net device!
    auto path = "/NodeList/" + std::to_string(nodeId) + "/DeviceList/0";

    this->dataSetValues = LinkedList_create();

    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
    LinkedList_add(dataSetValues, MmsValue_newBinaryTime(false));
    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

    libiec61850::CommParameters gooseCommParameters;

    gooseCommParameters.appId = 1000;
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

    this->i = 0;

    ns3::Simulator::Schedule(
        ns3::MilliSeconds(1000),
        &GOOSEPublisher::Send,
        this
        );
}

void
ns3::GOOSEPublisher::Send()
{
    if (this->i >= 4) return;

    if (this->i == 3) {
        /* now change dataset to send an invalid GOOSE message */
        LinkedList_add(dataSetValues, MmsValue_newBoolean(true));
        libiec61850::GoosePublisher_publish(
            this->publisher, dataSetValues
            );
    }
    else {
        if (libiec61850::GoosePublisher_publish(this->publisher, dataSetValues) == -1) {
            printf("Error sending message!\n");
        }
    }

    this->i++;

    ns3::Simulator::Schedule(
        ns3::MilliSeconds(1000),
        &GOOSEPublisher::Send,
        this
        );
}

void
ns3::GOOSEPublisher::StopApplication()
{
    NS_LOG_FUNCTION(this);
    libiec61850::GoosePublisher_destroy(this->publisher);
    LinkedList_destroyDeep(
        this->dataSetValues,
        reinterpret_cast<LinkedListValueDeleteFunction>(MmsValue_delete)
        );
}

