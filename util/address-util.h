//
// Created by lso on 12/02/25.
//

#ifndef ADDRESS_UTIL_H
#define ADDRESS_UTIL_H

#include <cstdint>
#include "ns3/mac48-address.h"
#include "ns3/packet-socket-address.h"

char *formatMacAddress(uint8_t mac[6]);

ns3::Mac48Address getMacAddress(uint8_t mac[6]);

ns3::PacketSocketAddress getPacketSocketAddress(ns3::Ptr<ns3::NetDevice>device, uint8_t destMac[6]);

#endif //ADDRESS_UTIL_H
