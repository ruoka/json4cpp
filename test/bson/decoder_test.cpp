#include <string>
#include <gtest/gtest.h>
#include "bson/model.hpp"

using namespace std::literals::string_literals;

TEST(DecoderTest,Mockup1)
{
    long i{1};
    ASSERT_EQ(1, (i & 0xFF));

    long ii{256};
    ASSERT_EQ(0, (ii & 0xFF));
    ASSERT_NE(256, (ii & 0xFF));
}

TEST(DecoderTest,Mockup2)
{
    bson::decoder d;
    d.decode(bson::byte_type{'1'});
    EXPECT_EQ('1', *reinterpret_cast<bson::byte_type*>(d.data()));
}

TEST(DecoderTest,Mockup3)
{
    bson::decoder d;
    d.decode(bson::int32_type{2});
    EXPECT_EQ(2, *reinterpret_cast<bson::int32_type*>(d.data()));
}

TEST(DecoderTest,Mockup4)
{
    bson::decoder d;
    d.decode(bson::int64_type{3});
    EXPECT_EQ(3, *reinterpret_cast<bson::int64_type*>(d.data()));
}

TEST(DecoderTest,Mockup5)
{
    bson::decoder d;
    d.decode(bson::double_type{4.0});
    EXPECT_EQ(4.0, *reinterpret_cast<bson::double_type*>(d.data()));
}

TEST(DecoderTest,Mockup6)
{
    bson::decoder d;
    d.decode("Ruoka"s);
    EXPECT_STREQ("Ruoka",d.data());
}

TEST(DecoderTest,Mockup7)
{
    bson::double_type b1;
    bson::string_type b2;
    bson::document_type b3;
    bson::array_type b4;
    bson::binary_type b5;
    struct foo {} b6;
    bson::objectid_type b7;
    bson::int32_type b16;
    bson::int64_type b18;

    std::clog << bson::type(b1) << "\n"
              << bson::type(b2) << "\n"
              << bson::type(b3) << "\n"
              << bson::type(b4) << "\n"
              << bson::type(b5) << "\n"
              << bson::type(b6) << "\n"
              << bson::type(b7) << "\n"
              << bson::type(false) << "\n"
              << bson::type(true) << "\n"
              << bson::type(nullptr) << "\n"
              << bson::type(b16) << "\n"
              << bson::type(b18) << "\n";
}
