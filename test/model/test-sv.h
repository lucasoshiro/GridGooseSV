#ifndef TEST_SV_H
#define TEST_SV_H

#include "ns3/test.h"

class TestSVProtection : ns3::TestCase
{
public:
    TestSVProtection() : ns3::TestCase("TestSVProtection") {};

private:
    void DoRun() override;
};

#endif //TEST_SV_H
