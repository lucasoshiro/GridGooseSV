
// Include a header file from your module to test.
#include "ns3/libiec61850-ns3.h"

// An essential include is test.h
#include "model/libiec61850/hal/thread/test-thread.h"
#include "model/libiec61850/hal/time/test-time.h"

#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// Add a doxygen group for tests.
// If you have more than one test, this should be in only one of them.
/**
 * \defgroup libiec61850-ns3-tests Tests for libiec61850-ns3
 * \ingroup libiec61850-ns3
 * \ingroup tests
 */

// This is an example TestCase.
/**
 * \ingroup libiec61850-ns3-tests
 * Test case for feature 1
 */
class Libiec61850Ns3TestCase1 : public TestCase
{
  public:
    Libiec61850Ns3TestCase1();
    virtual ~Libiec61850Ns3TestCase1();

  private:
    void DoRun() override;
};

// Add some help text to this case to describe what it is intended to test
Libiec61850Ns3TestCase1::Libiec61850Ns3TestCase1()
    : TestCase("Libiec61850Ns3 test case (does nothing)")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
Libiec61850Ns3TestCase1::~Libiec61850Ns3TestCase1()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
Libiec61850Ns3TestCase1::DoRun()
{
    // A wide variety of test macros are available in src/core/test.h
    NS_TEST_ASSERT_MSG_EQ(true, true, "true doesn't equal true for some reason");
    // Use this one for floating point comparisons
    NS_TEST_ASSERT_MSG_EQ_TOL(0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined

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
    AddTestCase(reinterpret_cast<TestCase*>(new HalTimeTest), TestCase::Duration::QUICK);
    AddTestCase(reinterpret_cast<TestCase*>(new TestThread), TestCase::Duration::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
/**
 * \ingroup libiec61850-ns3-tests
 * Static variable for test initialization
 */
static Libiec61850Ns3TestSuite slibiec61850Ns3TestSuite;
