#include "sv-helper.h"
#include "ns3/sv-publisher-app.h"
#include "ns3/sv-subscriber-app.h"

ns3::SVPublisherHelper::
SVPublisherHelper()
    : ApplicationHelper(ns3::SVPublisher::GetTypeId())
{
}

ns3::SVSubscriberHelper::
SVSubscriberHelper()
    : ApplicationHelper(ns3::SVSubscriber::GetTypeId())
{
}