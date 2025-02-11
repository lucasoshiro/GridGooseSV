//
// Created by lso on 11/02/25.
//

#include "device-util.h"

#include "../../../src/network/model/node.h"

#include "ns3/node-list.h"

namespace ns3
{

Ptr<ns3::NetDevice>
GetNetDevice(std::string path)
{
    // TODO: write assertions!

    const int firstSlash = path.find('/', 1);
    const int secondSlash = path.find('/', firstSlash + 1);

    const int nodeIndex = std::stoi(path.substr(firstSlash + 1, secondSlash - firstSlash - 1));

    const int lastSlash = path.find_last_of('/');
    const int deviceIndex = std::stoi(path.substr(lastSlash + 1));

    auto node = ns3::NodeList::GetNode(nodeIndex);
    auto device = node->GetDevice(deviceIndex);

    return device;
}

}