#include "test-address-util.h"
#include "ns3/address-util.h"

void
TestFormatAddress::DoRun()
{
    uint8_t address[] = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0x06};

    auto formatted = std::string(formatMacAddress(address));
    auto expected = "a1:b2:c3:d4:e5:06";

    NS_TEST_ASSERT_MSG_EQ(formatted, expected, "Expected addresss to be a1:b2:c3:d4:e5:06");
}

void
TestGetMacAddress::DoRun()
{
    uint8_t address[] = {0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0x06};
    auto mac48addr = getMacAddress(address);

    uint8_t buffer[6];
    mac48addr.CopyTo(buffer);

    auto expected = "a1:b2:c3:d4:e5:06x";

    NS_TEST_ASSERT_MSG_EQ(mac48addr, expected, buffer);
}