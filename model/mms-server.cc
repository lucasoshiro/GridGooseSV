//
// Created by lso on 05/02/25.
//

#include "mms-server.h"
#include "ns3/hal_time.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MMSServer");
NS_OBJECT_ENSURE_REGISTERED(MmsServer);

TypeId
MmsServer::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::MmsServer")
            .SetParent<Application>()
            .SetGroupName("Applications")
            .AddConstructor<MmsServer>();

    return tid;
}

MmsServer::MmsServer()
{
    NS_LOG_FUNCTION(this);
}

MmsServer::~MmsServer()
{
    NS_LOG_FUNCTION(this);
}

void
MmsServer::StartApplication()
{

}

void
MmsServer::StopApplication()
{

}

