#include "ethernet-client.h"

#include "ns3/core-module.h"
#include "ns3/hal_ethernet.h"
#include "ns3/packet-socket-client.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("EthernetClient");
NS_OBJECT_ENSURE_REGISTERED(EthernetClient);

TypeId
EthernetClient::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::EthernetClient")
            .SetParent<Application>()
            .SetGroupName("LibIEC61850")
            .AddConstructor<EthernetClient>();
    return tid;
}

EthernetClient::EthernetClient()
{
    NS_LOG_FUNCTION(this);
    ethSocket = nullptr;
}

void
EthernetClient::SetServerAddress(PacketSocketAddress serverAddr)
{
    NS_LOG_FUNCTION(this);
    this->serverAddr = serverAddr;
}

void
EthernetClient::StartApplication()
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

        uint8_t destAddress[20];
        this->serverAddr.GetPhysicalAddress().CopyTo(destAddress);

        this->ethSocket = libiec61850::Ethernet_createSocket(
            reinterpret_cast<char *>(interfaceId),
            destAddress
            );
    }

    Simulator::ScheduleNow(&EthernetClient::Send, this);
}

void
EthernetClient::StopApplication()
{
    NS_LOG_FUNCTION(this);
    libiec61850::Ethernet_destroySocket(this->ethSocket);
}

void
EthernetClient::Send()
{
    NS_LOG_FUNCTION(this);
    uint8_t bla[] = "foobar";

    libiec61850::Ethernet_sendPacket(
        this->ethSocket,
        bla,
        strlen(reinterpret_cast<char *>(bla))
        );
}

}
