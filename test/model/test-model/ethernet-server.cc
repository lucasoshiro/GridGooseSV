#include "ethernet-server.h"

#include "ns3/core-module.h"
#include "ns3/hal_ethernet.h"
#include "ns3/packet-socket-client.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("EthernetServer");
NS_OBJECT_ENSURE_REGISTERED(EthernetServer);

TypeId
EthernetServer::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::EthernetServer")
            .SetParent<Application>()
            .SetGroupName("GridGooseSV")
            .AddConstructor<EthernetServer>();
    return tid;
}

EthernetServer::EthernetServer()
{
    NS_LOG_FUNCTION(this);
    ethSocket = nullptr;
}

void
EthernetServer::StartApplication()
{
    NS_LOG_FUNCTION(this);

    if (this->ethSocket == nullptr)
    {
        const auto nodePath = (
            "/NodeList/" +
            std::to_string(this->GetNode()->GetId()) +
            "/DeviceList/0"
            );

        uint8_t interfaceId[128];

        std::strcpy(
            reinterpret_cast<char *>(interfaceId),
            nodePath.c_str()
            );

        this->ethSocket = libiec61850::Ethernet_createSocket(
            reinterpret_cast<char *>(interfaceId),
            nullptr
            );
    }
}

void
EthernetServer::StopApplication()
{
    NS_LOG_FUNCTION(this);
    libiec61850::Ethernet_destroySocket(this->ethSocket);
}
}
