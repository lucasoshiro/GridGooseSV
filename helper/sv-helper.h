#ifndef SV_HELPER_H
#define SV_HELPER_H

#include "ns3/application-helper.h"

namespace ns3
{

class SVPublisherHelper : public ApplicationHelper
{
public:
    SVPublisherHelper();
};

class SVSubscriberHelper : public ApplicationHelper
{
public:
    SVSubscriberHelper();
};

}
#endif //SV_HELPER_H
