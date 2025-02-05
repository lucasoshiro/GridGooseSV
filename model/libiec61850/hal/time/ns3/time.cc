#include "ns3/hal_time.h"
#include "ns3/core-module.h"

PAL_API msSinceEpoch
Hal_getTimeInMs(void)
{
    return ns3::Simulator::Now().GetMilliSeconds();
}

PAL_API nsSinceEpoch
Hal_getTimeInNs(void)
{
    return ns3::Simulator::Now().GetNanoSeconds();
}

PAL_API bool
Hal_setTimeInNs(nsSinceEpoch nsTime)
{
    // TODO: not used, only too keep compatibility without changing the .h file
    return false;
}

