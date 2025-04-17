#ifndef ETHERNET_CLIENT_H
#define ETHERNET_CLIENT_H

#include "ns3/application.h"
#include "ns3/hal_ethernet.h"
#include "ns3/packet-socket-address.h"
#include "ns3/socket.h"

namespace ns3 {

class EthernetClient : public Application
{
public:
  EthernetClient();

  static TypeId GetTypeId();
  void SetServerAddress(PacketSocketAddress serverAddr);

private:
  void StartApplication() override;
  void StopApplication() override;

  void Send();

  libiec61850::EthernetSocket ethSocket;
  PacketSocketAddress serverAddr;
};

} // namespace ns3

#endif //ETHERNET_CLIENT_H
