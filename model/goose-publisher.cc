#include "goose-publisher.h"

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
}

void
ns3::GOOSEPublisher::StopApplication()
{
    NS_LOG_FUNCTION(this);
}