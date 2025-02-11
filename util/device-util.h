#ifndef DEVICE_UTIL_H
#define DEVICE_UTIL_H

#include "ns3/core-module.h"
#include "ns3/net-device.h"

namespace ns3
{

// TODO: write a real documentation
// Get a path in format /NodeList/<n>/DeviceList/<m>. Doesn't work with other formats
Ptr<ns3::NetDevice> GetNetDevice(std::string path);

}

#endif //DEVICE_UTIL_H
