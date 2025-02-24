#ifndef TEST_SV_H
#define TEST_SV_H

#include "ns3/test.h"

class TestSV : ns3::TestCase
{
  public:
    TestSV() : ns3::TestCase("TestSV") {};
private:
    void DoRun() override;
};

#endif //TEST_SV_H
