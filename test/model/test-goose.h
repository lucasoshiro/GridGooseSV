#ifndef TEST_GOOSE_H
#define TEST_GOOSE_H

#include "ns3/test.h"

class TestGOOSE : ns3::TestCase
{
  public:
    TestGOOSE() : ns3::TestCase("TestGOOSE") {};

  private:
    void DoRun() override;
};

#endif //TEST_GOOSE_H
