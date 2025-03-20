#include "model/libiec61850/hal/filesystem/test-filesystem.h"
#include "model/libiec61850/hal/thread/test-thread.h"
#include "model/libiec61850/hal/time/test-time.h"
#include "model/test-ethernet.h"
#include "model/test-goose.h"
#include "model/test-sv.h"
#include "util/test-address-util.h"
#include "util/test-device-util.h"

#include "ns3/test.h"

using namespace ns3;

/**
 * \defgroup libiec61850-ns3-tests Tests for libiec61850-ns3
 * \ingroup libiec61850-ns3
 * \ingroup tests
 */

/**
 * \ingroup libiec61850-ns3-tests
 * TestSuite for module libiec61850-ns3
 */
class Libiec61850Ns3TestSuite : public TestSuite
{
  public:
    Libiec61850Ns3TestSuite();
};

Libiec61850Ns3TestSuite::Libiec61850Ns3TestSuite()
    : TestSuite("libiec61850-ns3", Type::UNIT)
{
    TestCase *testCases[] = {
        reinterpret_cast<TestCase*>(new HalTimeTest),
        reinterpret_cast<TestCase*>(new TestThread),
        reinterpret_cast<TestCase*>(new TestFilesystem),
        reinterpret_cast<TestCase*>(new TestGetDevice),
        reinterpret_cast<TestCase*>(new TestGetNode),
        reinterpret_cast<TestCase*>(new TestFormatAddress),
        reinterpret_cast<TestCase*>(new TestGetMacAddress),
        reinterpret_cast<TestCase*>(new TestGetPacketSocketAddress),
        reinterpret_cast<TestCase*>(new TestEthernet),
        reinterpret_cast<TestCase*>(new TestGetInterfaceMACAddress),
        reinterpret_cast<TestCase*>(new TestGOOSE),
        reinterpret_cast<TestCase*>(new TestSV)
    };

    for (TestCase *testCase : testCases)
    {
        AddTestCase(testCase, TestCase::Duration::QUICK);
    }
}

/**
 * \ingroup libiec61850-ns3-tests
 * Static variable for test initialization
 */
static Libiec61850Ns3TestSuite slibiec61850Ns3TestSuite;
