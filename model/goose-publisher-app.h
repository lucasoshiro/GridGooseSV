#ifndef GOOSE_PUBLISHER_H
#define GOOSE_PUBLISHER_H

#include "ns3/application.h"
#include "ns3/linked_list.h"
#include "ns3/goose_publisher.h"
#include "ns3/traced-value.h"

namespace ns3
{

class GOOSEPublisher : public Application
{
public:
    GOOSEPublisher();
    static TypeId GetTypeId();

private:
    void StartApplication() override;
    void StopApplication() override;

    void Send();
    void SendEvent();

    ns3::Time T(uint8_t n);

    LinkedList dataSetValues;
    libiec61850::GoosePublisher publisher;

    uint64_t deviceIndex;
    uint64_t count;
    uint8_t eventMessages;
    uint8_t eventCount;
    bool sendEvents;

    uint16_t appId;
    ns3::EventId eventId;
    ns3::EventId gooseEventId;
    ns3::Time t0;
    ns3::Time t1;

    ns3::Time eventInterval;
    ns3::TracedValue<uint64_t> sent;
};

}
#endif //GOOSE_PUBLISHER_H
