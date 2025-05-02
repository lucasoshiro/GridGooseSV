#ifndef SV_APPS_H
#define SV_APPS_H

#include "ns3/application.h"
#include "ns3/hal_ethernet.h"
#include "ns3/packet-socket-address.h"
#include "ns3/traced-value.h"
#include "libiec61850/src/sampled_values/sv_publisher.h"

namespace ns3 {

class SVPublisher : public Application
{
public:
    SVPublisher();
    static TypeId GetTypeId();

private:
    void StartApplication() override;
    void StopApplication() override;

    void Measure();
    void Send();
    void UpdateValues();

    libiec61850::EthernetSocket ethSocket;
    libiec61850::SVPublisher svPublisher;

    int offsets[8][8];
    libiec61850::SVPublisher_ASDU asdus[8];
    float vals[8][8];
    int tss[8];

    uint64_t count;
    uint64_t deviceIndex;
    uint64_t frequency;
    uint64_t samplesPerCycle;
    uint8_t samplesPerMessage;
    uint8_t sampleIndex;

    ns3::EventId eventId;

    ns3::TracedValue<uint64_t> sent;
    ns3::Time interval;
};

} // namespace ns3


#endif //SV_APPS_H
