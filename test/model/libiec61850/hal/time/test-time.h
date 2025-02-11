#ifndef TEST_TIME_H
#define TEST_TIME_H

#include "ns3/test.h"

class HalTimeTest : ns3::TestCase
{
  public:
    HalTimeTest() : TestCase("HalTimeTest"){};
    void scheduled();

  private :
    void DoRun() override;
};

#endif //TEST_TIME_H
