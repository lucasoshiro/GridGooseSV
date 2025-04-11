#ifndef GOOSE_PUBLISHER_H
#define GOOSE_PUBLISHER_H

#include "ns3/application.h"
#include "ns3/linked_list.h"
#include "ns3/goose_publisher.h"

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
    int i;
    LinkedList dataSetValues;
    libiec61850::GoosePublisher publisher;

    uint64_t deviceIndex;
};

}
#endif //GOOSE_PUBLISHER_H
