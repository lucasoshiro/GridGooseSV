#ifndef TEST_DEVICE_UTIL_H
#define TEST_DEVICE_UTIL_H

#include "ns3/test.h"

class TestGetDevice : ns3::TestCase
{
  public:
    TestGetDevice() : TestCase("TestGetDevice") {}

  private:
    void DoRun() override;
};

class TestGetNode : ns3::TestCase
{
public:
    TestGetNode() : TestCase("TestGetNode") {}

private:
    void DoRun() override;
};

#endif //TEST_DEVICE_UTIL_H
