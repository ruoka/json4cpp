#include <sstream>
#include <gtest/gtest.h>
#include "xson/fast/encoder.hpp"
#include "xson/fast/decoder.hpp"

using namespace std::string_literals;
using namespace xson::fast;

TEST(XsonFastTest, Byte)
{
    auto ss= std::stringstream{};

    auto b1 = std::uint8_t{1};
    xson::fast::encode(ss,b1);

    auto b2 =  std::uint8_t{0};
    xson::fast::decode(ss,b2);

    ASSERT_EQ(b1, b2);
}

TEST(XsonFastTest, UInteger32)
{
    auto ss= std::stringstream{};

    auto i1 = std::uint32_t{1234567};
    xson::fast::encode(ss,i1);

    auto i2 = std::uint32_t{0};
    xson::fast::decode(ss,i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, Integer32)
{
    auto ss= std::stringstream{};

    auto i1 = std::int32_t{-1234567};
    xson::fast::encode(ss,i1);

    auto i2 = std::int32_t{0};
    xson::fast::decode(ss,i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, UInteger64)
{
    auto ss= std::stringstream{};

    auto i1 = std::uint64_t{1234567};
    xson::fast::encode(ss,i1);

    auto i2 = std::uint64_t{0};
    xson::fast::decode(ss,i2);

    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, Integer64)
{
    auto ss= std::stringstream{};

    auto i1 = std::int64_t{-1234567898765432};
    xson::fast::encode(ss,i1);

    auto i2 = std::int64_t{1};
    xson::fast::decode(ss,i2);
    ASSERT_EQ(i1, i2);
}

TEST(XsonFastTest, String)
{
    auto ss= std::stringstream{};

    const auto s1 = "1234567 Kaius Ruokonen \n\t x"s;
    xson::fast::encode(ss,s1);

    auto s2 = ""s;
    xson::fast::decode(ss,s2);

    ASSERT_EQ(s1, s2);
}
