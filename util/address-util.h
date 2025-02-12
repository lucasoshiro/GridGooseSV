//
// Created by lso on 12/02/25.
//

#ifndef ADDRESS_UTIL_H
#define ADDRESS_UTIL_H

#include <cstdint>
#include "ns3/mac48-address.h"

char *formatMacAddress (uint8_t *mac);

ns3::Mac48Address getMacAddress (uint8_t *mac);

#endif //ADDRESS_UTIL_H
