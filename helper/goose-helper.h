#ifndef GOOSE_HELPER_H
#define GOOSE_HELPER_H

#include "ns3/application-helper.h"

namespace ns3
{

class GOOSEPublisherHelper : public ApplicationHelper
{
public:
    GOOSEPublisherHelper();
};

class GOOSEReceiverHelper : public ApplicationHelper
{
public:
    GOOSEReceiverHelper();
};

}

#endif //GOOSE_HELPER_H
