#ifndef TEST_FILESYSTEM_H
#define TEST_FILESYSTEM_H

#include "ns3/test.h""

class TestFilesystem : ns3::TestCase
{
  public:
    TestFilesystem() : ns3::TestCase("TestFilesystem") {};

  private:
    void DoRun () override;
};

#endif //TEST_FILESYSTEM_H
