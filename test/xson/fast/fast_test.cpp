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

    auto b1 = std::uint8_t{1};
    ncdr.encode(b1);

    auto b2 =  std::uint8_t{0};
    dcdr.decode(b2);

    ASSERT_EQ(b1, b2);
}

TEST(XsonFastTest, UInteger32)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = std::uint32_t{1234567};
    ncdr.encode(i1);

    auto i2 = std::uint32_t{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, Integer32)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = std::int32_t{-1234567};
    ncdr.encode(i1);

    auto i2 = std::int32_t{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, UInteger64)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = std::uint64_t{1234567};
    ncdr.encode(i1);

    auto i2 = std::uint64_t{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, Integer64)
{
    auto ss= std::stringstream{};
    auto ncdr = encoder{ss};
    auto dcdr = decoder{ss};

    auto i1 = std::int64_t{-1234567898765432};
    ncdr.encode(i1);

    auto i2 = std::int64_t{1};
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
