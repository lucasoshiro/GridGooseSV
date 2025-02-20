#ifndef ETHERNET_SERVER_H
#define ETHERNET_SERVER_H

#include "ns3/application.h"
#include "ns3/hal_ethernet.h"

namespace ns3 {

class EthernetServer : public Application
{
public:
    EthernetServer();

    static TypeId GetTypeId();

private:
    void StartApplication() override;
    void StopApplication() override;

    libiec61850::EthernetSocket ethSocket;
};

} // namespace ns3


#endif //ETHERNET_SERVER_H
