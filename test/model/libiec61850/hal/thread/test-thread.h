#include "ns3/test.h"

#ifndef TEST_THREAD_H
#define TEST_THREAD_H

class TestThread : ns3::TestCase
{
  public:
    TestThread() : ns3::TestCase("TestThread") {};

  private:
    void DoRun() override;
};

#endif //TEST_THREAD_H
