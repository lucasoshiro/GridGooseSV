//
// Created by lso on 10/02/25.
//

#include "test-time.h"

#include "ns3/core-module.h"
#include "ns3/hal_time.h"

using namespace ns3;

static void run_scheduled(HalTimeTest *test)
{
    test->scheduled();
}

void
HalTimeTest::scheduled()
{
    auto ns = libiec61850::Hal_getTimeInNs();
    auto ms = libiec61850::Hal_getTimeInMs();

    NS_TEST_ASSERT_MSG_EQ(ns, 42000000, "Expected nsSinceEpoch to be 42000000");
    NS_TEST_ASSERT_MSG_EQ(ms, 42, "Expected msSinceEpoch to be 42");
}

void
HalTimeTest::DoRun()
{
    auto ns1 = libiec61850::nsSinceEpoch();
    auto ms1 = libiec61850::msSinceEpoch();

    NS_TEST_ASSERT_MSG_EQ(ns1, 0, "Expected nsSinceEpoch to be 0");
    NS_TEST_ASSERT_MSG_EQ(ms1, 0, "Expected msSinceEpoch to be 0");

    Simulator::Schedule(MilliSeconds(42), run_scheduled, this);
    Simulator::Run();
}