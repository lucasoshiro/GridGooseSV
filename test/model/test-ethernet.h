#ifndef TEST_ETHERNET_H
#define TEST_ETHERNET_H

#include "ns3/test.h"

class TestEthernet : public ns3::TestCase
{
public:
  TestEthernet() : ns3::TestCase ("TestEthernet") {};
private:
  void DoRun() override;
};

#endif //TEST_ETHERNET_H
