#include "ns3/address-util.h"
#include "ns3/core-module.h"
#include "ns3/device-util.h"
#include "ns3/hal_ethernet.h"
#include "ns3/node.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/packet-socket.h"

namespace libiec61850
{

struct sEthernetSocket
{
    ns3::Ptr<ns3::Socket> packetSocket;
    bool isBind;
    ns3::Address address;
};

struct sEthernetHandleSet
{
    std::set<EthernetSocket> handleset;
};

PAL_API EthernetHandleSet
EthernetHandleSet_new(void)
{
    return new sEthernetHandleSet;
}

PAL_API void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if (sock != nullptr)
    {
        self->handleset.insert(sock);
    }
}

PAL_API void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    if (sock != nullptr)
    {
        self->handleset.erase(sock);
    }
}

PAL_API int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
    // TODO: Not implemented. Probably won't be useful in an simulation environment
    return -1;
}

PAL_API void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
    delete self;
}

PAL_API void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
    const std::string path(interfaceId);
    auto device = ns3::GetNetDevice(path);
    auto address = device->GetAddress();

    uint8_t mac[20];
    address.CopyTo(mac);
    memcpy(addr, &mac[0], 6);
}

PAL_API EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{
    const auto socket = new sEthernetSocket();
    const std::string path(interfaceId);
    const auto device = ns3::GetNetDevice(path);

    const auto node = device->GetNode();

    const auto packetSocket = ns3::Socket::CreateSocket(
        node,
        ns3::TypeId::LookupByName("ns3::PacketSocketFactory")
        );

    if (destAddress != nullptr)
    {
        const auto remoteAddress = getPacketSocketAddress(device, destAddress);
        packetSocket->Bind(remoteAddress);
        packetSocket->Connect(remoteAddress);
    }
    else
    {
        auto localAddr = ns3::PacketSocketAddress();
        localAddr.SetPhysicalAddress(device->GetAddress());

        // TODO: shouldn't use localAddr?
        packetSocket->Bind();
    }

    socket->packetSocket = packetSocket;

    return socket;
}

PAL_API void
Ethernet_destroySocket(EthernetSocket ethSocket)
{
    ethSocket->packetSocket->Close();
    delete ethSocket;
}

PAL_API void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize)
{
    const auto packet = ns3::Create<ns3::Packet>(
        buffer,
        static_cast<uint32_t>(packetSize)
        );

    ethSocket->packetSocket->Send(packet);
}

PAL_API void
Ethernet_setMode(EthernetSocket ethSocket, EthernetSocketMode mode)
{
    // Not implemented
}

PAL_API void
Ethernet_addMulticastAddress(EthernetSocket ethSocket, uint8_t* multicastAddress)
{
    // Not implemented
}

PAL_API void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
    // Not implemented
}

PAL_API int
Ethernet_receivePacket(EthernetSocket ethSocket, uint8_t* buffer, int bufferSize)
{
    // TODO: libiec61850 does something with bind before this. Check it.
    auto packet = ethSocket->packetSocket->Recv(bufferSize, 0);
    return packet->CopyData(buffer, 0);
}

PAL_API bool
Ethernet_isSupported(void)
{
    return true;
}
}