#ifndef GOOSE_PUBLISHER_H
#define GOOSE_PUBLISHER_H

#include "ns3/application.h"

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
};

}
#endif //GOOSE_PUBLISHER_H
