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
    class _SVSample {
    public:
        _SVSample():
        sampleTimestamp(0),
        receivedTimestamp(0),
        smpCnt(0),
        ia(0),
        ib(0),
        ic(0) {};

        uint64_t sampleTimestamp; // nanoseconds
        uint64_t receivedTimestamp; // nanoseconds

        uint16_t smpCnt;
        double ia;
        double ib;
        double ic;
    };

    SVSubscriber();
    static TypeId GetTypeId();
    void Receive(libiec61850::SVSubscriber_ASDU asdu);
    _SVSample GetLastSample() const;

private:
    void StartApplication() override;
    void StopApplication() override;

    libiec61850::SVReceiver receiver;
    libiec61850::SVSubscriber subscriber;

    uint64_t deviceIndex;

    ns3::TracedValue<uint64_t> received;

    _SVSample lastSample;
};


}

#endif //SV_SUBSCRIBER_H
