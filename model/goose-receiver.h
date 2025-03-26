#ifndef GOOSE_RECEIVER_H
#define GOOSE_RECEIVER_H

#include "ns3/application.h"
#include "ns3/goose_receiver.h"

namespace ns3
{
class GOOSEReceiver : public Application
{
public:
    GOOSEReceiver();
    static ns3::TypeId GetTypeId();

private:
    void StartApplication() override;
    void StopApplication() override;

    libiec61850::GooseReceiver receiver;
};


}

#endif //GOOSE_RECEIVER_H
