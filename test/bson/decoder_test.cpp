#include <gtest/gtest.h>
#include "bson/decoder.hpp"

using namespace std::literals::string_literals;

TEST(DecoderTest,Mockup)
{
    long i{1};
    ASSERT_EQ(1, (i & 0xFF));

    long ii{256};
    ASSERT_EQ(0, (ii & 0xFF));
    ASSERT_NE(256, (ii & 0xFF));

    bson::decoder d;

    char bytex1[1];
    d.buffer = bytex1;
    d.decode(bson::byte_type{'1'});
    EXPECT_EQ('1', static_cast<bson::byte_type>(*bytex1));

    char bytex4[4];
    d.buffer = bytex4;
    d.decode(bson::int32_type{2});
    EXPECT_EQ(2, *reinterpret_cast<bson::int32_type*>(bytex4));

    char bytex8v1[8];
    d.buffer = bytex8v1;
    d.decode(bson::int64_type{3});
    EXPECT_EQ(3, *reinterpret_cast<bson::int64_type*>(bytex8v1));

    char bytex8v2[8];
    d.buffer = bytex8v2;
    d.decode(bson::double_type{4.0});
    EXPECT_EQ(4.0, *reinterpret_cast<bson::double_type*>(bytex8v2));

    char bytex10[10];
    d.buffer = bytex10;
    d.decode("Ruoka"s);
    EXPECT_STREQ("Ruoka",bytex10);
}
