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

  public:
    void DoRun() override;
};

#endif //TEST_ADDRESS_UTIL_H
