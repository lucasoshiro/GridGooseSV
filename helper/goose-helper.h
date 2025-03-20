#ifndef GOOSE_HELPER_H
#define GOOSE_HELPER_H

#include "ns3/application-helper.h"
#include "ns3/goose-publisher.h"

namespace ns3
{

class GOOSEPublisherHelper : public ApplicationHelper
{
public:
    GOOSEPublisherHelper();
};

}

#endif //GOOSE_HELPER_H
