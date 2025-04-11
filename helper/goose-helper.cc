//
// Created by lso on 20/03/25.
//

#include "goose-helper.h"
#include "ns3/goose-publisher-app.h"
#include "ns3/goose-receiver-app.h"

ns3::GOOSEPublisherHelper::
GOOSEPublisherHelper()
    : ns3::ApplicationHelper(ns3::GOOSEPublisher::GetTypeId())
{
}

ns3::GOOSEReceiverHelper::
GOOSEReceiverHelper()
    : ns3::ApplicationHelper(ns3::GOOSEReceiver::GetTypeId())
{
}