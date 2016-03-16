#include <sstream>
#include <gtest/gtest.h>
#include "xson/fast/encoder.hpp"
#include "xson/fast/decoder.hpp"

using namespace std::string_literals;
using namespace xson::fast;

TEST(FastTest, Byte)
{
    std::stringstream ss;
    encoder ncdr{ss};
    decoder dcdr{ss};

    const std::uint8_t b1{1};
    ncdr.encode(b1);

    std::uint8_t b2{0};
    dcdr.decode(b2);

    ASSERT_EQ(b1, b2);
}

TEST(FastTest, UInteger32)
{
    std::stringstream ss;
    encoder ncdr{ss};
    decoder dcdr{ss};

    const std::uint32_t i1{1234567};
    ncdr.encode(i1);

    std::uint32_t i2{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(FastTest, Integer32)
{
    std::stringstream ss;
    encoder ncdr{ss};
    decoder dcdr{ss};

    const std::int32_t i1{-1234567};
    ncdr.encode(i1);

    std::int32_t i2{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(FastTest, UInteger64)
{
    std::stringstream ss;
    encoder ncdr{ss};
    decoder dcdr{ss};

    const std::uint64_t i1{1234567};
    ncdr.encode(i1);

    std::uint64_t i2{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(FastTest, Integer64)
{
    std::stringstream ss;
    encoder ncdr{ss};
    decoder dcdr{ss};

    const std::int64_t i1{-12345678987654321};
    ncdr.encode(i1);

    std::int64_t i2{0};
    dcdr.decode(i2);

    ASSERT_EQ(i1, i2);
}

TEST(FastTest, String)
{
    std::stringstream ss;
    encoder ncdr{ss};
    decoder dcdr{ss};

    const std::string s1{"1234567 Kaius Ruokonen \n\t x"};
    ncdr.encode(s1);

    std::string s2{};
    dcdr.decode(s2);

    ASSERT_EQ(s1, s2);
}
