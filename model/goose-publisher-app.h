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
    LinkedList dataSetValues;
    libiec61850::GoosePublisher publisher;

    uint64_t deviceIndex;
    uint64_t count;

    ns3::TracedValue<uint64_t> sent;
};

}
#endif //GOOSE_PUBLISHER_H
