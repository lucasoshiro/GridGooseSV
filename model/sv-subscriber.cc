#include "sv-subscriber.h"

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
}

void
ns3::SVSubscriber::StopApplication()
{
    NS_LOG_FUNCTION(this);
}