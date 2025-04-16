#ifndef SV_SUBSCRIBER_H
#define SV_SUBSCRIBER_H

#include "ns3/application.h"
#include "ns3/sv_subscriber.h"
#include "ns3/traced-value.h"

namespace ns3
{

class SVSubscriber : public Application
{
public:
    SVSubscriber();
    static TypeId GetTypeId();
    void Receive(libiec61850::SVSubscriber_ASDU asdu);

private:
    void StartApplication() override;
    void StopApplication() override;

    libiec61850::SVReceiver receiver;
    libiec61850::SVSubscriber subscriber;

    uint64_t deviceIndex;

    ns3::TracedValue<uint64_t> received;
    ns3::TracedValue<uint16_t> lastSmpCnt;

    ns3::TracedValue<uint64_t> mts;
    ns3::TracedValue<uint64_t> rts;

    ns3::TracedValue<double> ia;
    ns3::TracedValue<double> ib;
    ns3::TracedValue<double> ic;
};


}

#endif //SV_SUBSCRIBER_H
