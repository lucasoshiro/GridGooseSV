#ifndef SV_SUBSCRIBER_H
#define SV_SUBSCRIBER_H
#include "../../../src/network/model/application.h"

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
};


}

#endif //SV_SUBSCRIBER_H
