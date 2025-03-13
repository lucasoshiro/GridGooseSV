#ifndef SV_SUBSCRIBER_H
#define SV_SUBSCRIBER_H

#include "ns3/application.h"
#include "ns3/sv_subscriber.h"

namespace ns3
{

class SVSubscriber : public Application
{
public:
    SVSubscriber();
    static TypeId GetTypeId();

private:
    void StartApplication() override;
    void StopApplication() override;

    libiec61850::SVReceiver receiver;
    libiec61850::SVSubscriber subscriber;
};


}

#endif //SV_SUBSCRIBER_H
