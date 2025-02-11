#ifndef TEST_DEVICE_UTIL_H
#define TEST_DEVICE_UTIL_H

#include "ns3/test.h"

class TestDeviceUtil : ns3::TestCase
{
  public:
    TestDeviceUtil() : TestCase("TestDeviceUtil") {}

  private:
    void DoRun() override;
};

#endif //TEST_DEVICE_UTIL_H
