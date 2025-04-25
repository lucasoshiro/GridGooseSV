#ifndef GOOSE_RECEIVER_H
#define GOOSE_RECEIVER_H

#include "ns3/application.h"
#include "ns3/goose_receiver.h"
#include "ns3/traced-value.h"

namespace ns3
{
class GOOSEReceiver : public Application
{
public:
    GOOSEReceiver();
    static ns3::TypeId GetTypeId();
    void Receive(libiec61850::GooseSubscriber subscriber);

    ns3::Time lastReceivedTime;

private:
    void StartApplication() override;
    void StopApplication() override;

    libiec61850::GooseReceiver receiver;
    libiec61850::GooseSubscriber subscriber;

    uint64_t deviceIndex;
    ns3::TracedValue<uint64_t> received;
};


}

#endif //GOOSE_RECEIVER_H
