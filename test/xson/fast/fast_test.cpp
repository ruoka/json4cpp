#include <sstream>
#include <gtest/gtest.h>
#include "xson/fast/encoder.hpp"
#include "xson/fast/decoder.hpp"

using namespace std::string_literals;
using namespace xson::fast;

TEST(XsonFastTest, Byte)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto b1 = std::uint32_t{'y'};
    ncdr.encode(b1);

    auto b2 =  std::uint32_t{'n'};
    dcdr.decode(b2);

    ASSERT_EQ(b1, b2);
}

TEST(XsonFastTest, UInteger32)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = 1234567u;
    ncdr.encode(i1);

    auto i2 = 0u;
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, Integer32)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = -1234567;
    ncdr.encode(i1);

    auto i2 = 0;
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, UInteger64)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = 1234567ull;
    ncdr.encode(i1);

    auto i2 = 0ull;
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, Integer64)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = -12345678987654321ll;
    ncdr.encode(i1);

    auto i2 = 01ll;
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, String)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    const auto s1 = "1234567 Kaius Ruokonen \n\t x"s;
    ncdr.encode(s1);

    auto s2 = ""s;
    dcdr.decode(s2);

    ASSERT_EQ(s1, s2);
}
