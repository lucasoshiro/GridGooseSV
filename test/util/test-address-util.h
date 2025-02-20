//
// Created by lso on 12/02/25.
//

#ifndef TEST_ADDRESS_UTIL_H
#define TEST_ADDRESS_UTIL_H

#include "ns3/test.h"

class TestFormatAddress : ns3::TestCase
{
  public:
    TestFormatAddress() : TestCase("TestFormatAddress") {};

  private:
    void DoRun() override;
};

class TestGetMacAddress : ns3::TestCase
{
  public:
    TestGetMacAddress() : TestCase("TestGetMacAddress") {};

  private:
    void DoRun() override;
};

class TestGetPacketSocketAddress : ns3::TestCase
{
  public:
    TestGetPacketSocketAddress() : TestCase("TestGetPacketSocketAddress") {};

  private:
    void DoRun() override;
};

#endif //TEST_ADDRESS_UTIL_H
