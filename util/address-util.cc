//
// Created by lso on 12/02/25.
//

#include "address-util.h"

#include <format>

char *
formatMacAddress(uint8_t* mac)
{
    const auto s = new char[18];
    sprintf(
        s,
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0],
        mac[1],
        mac[2],
        mac[3],
        mac[4],
        mac[5]
        );

    return s;
}

ns3::Mac48Address
getMacAddress(uint8_t* mac)
{
    auto formatted = formatMacAddress(mac);
    return ns3::Mac48Address(formatted);
}