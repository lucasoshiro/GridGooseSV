#include "model/libiec61850/hal/filesystem/test-filesystem.h"
#include "model/libiec61850/hal/time/test-time.h"
#include "model/test-ethernet.h"
#include "model/test-goose.h"
#include "model/test-sv.h"
#include "util/test-address-util.h"
#include "util/test-device-util.h"

#include "ns3/test.h"

using namespace ns3;

/**
 * \defgroup gridgoosesv-tests Tests for GridGooseSV
 * \ingroup gridgoosesv
 * \ingroup tests
 */

/**
 * \ingroup gridgoosesv-tests
 * TestSuite for module gridgoosesv
 */
class GridGooseSVTestSuite : public TestSuite
{
  public:
    GridGooseSVTestSuite();
};

GridGooseSVTestSuite::GridGooseSVTestSuite()
    : TestSuite("gridgoosesv", Type::UNIT)
{
    TestCase *testCases[] = {
        reinterpret_cast<TestCase*>(new HalTimeTest),
        reinterpret_cast<TestCase*>(new TestFilesystem),
        reinterpret_cast<TestCase*>(new TestGetDevice),
        reinterpret_cast<TestCase*>(new TestGetNode),
        reinterpret_cast<TestCase*>(new TestFormatAddress),
        reinterpret_cast<TestCase*>(new TestGetMacAddress),
        reinterpret_cast<TestCase*>(new TestGetPacketSocketAddress),
        reinterpret_cast<TestCase*>(new TestEthernet),
        reinterpret_cast<TestCase*>(new TestGetInterfaceMACAddress),
        reinterpret_cast<TestCase*>(new TestGOOSE),
        reinterpret_cast<TestCase*>(new TestSVProtection),
        reinterpret_cast<TestCase*>(new TestSVMeasurement),
    };

    for (TestCase *testCase : testCases)
    {
        AddTestCase(testCase, TestCase::Duration::QUICK);
    }
}

/**
 * \ingroup gridgoosesv-tests
 * Static variable for test initialization
 */
static GridGooseSVTestSuite gridGooseSVTestSuite;
