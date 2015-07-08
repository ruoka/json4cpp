#include <string>
#include <gtest/gtest.h>
#include "bson/model.hpp"

using namespace std::literals::string_literals;

TEST(DecoderTest,Byte)
{
    bson::decoder d;
    d.decode(bson::byte_type{'1'});
    EXPECT_EQ('1', *reinterpret_cast<bson::byte_type*>(d.data()));
}

TEST(DecoderTest,Int32)
{
    bson::decoder d;
    d.decode(bson::int32_type{2});
    EXPECT_EQ(2, *reinterpret_cast<bson::int32_type*>(d.data()));
}

TEST(DecoderTest,Int64)
{
    bson::decoder d;
    d.decode(bson::int64_type{3});
    EXPECT_EQ(3, *reinterpret_cast<bson::int64_type*>(d.data()));
}

TEST(DecoderTest,Double)
{
    bson::decoder d;
    d.decode(bson::double_type{4.0});
    EXPECT_EQ(4.0, *reinterpret_cast<bson::double_type*>(d.data()));
}

TEST(DecoderTest,String)
{
    bson::decoder d;
    d.decode("Ruoka"s);
    EXPECT_STREQ("Ruoka",d.data());
}

TEST(DecoderTest,Boolean)
{
    bson::decoder d;
    d.decode(false);
    EXPECT_EQ(false, *reinterpret_cast<bson::boolean_type*>(d.data()));
}

TEST(DecoderTest,Date)
{
    auto val = std::chrono::system_clock::now();
    auto ref = std::chrono::system_clock::to_time_t(val);
    bson::decoder d;
    d.decode(val);
    EXPECT_EQ(ref, *reinterpret_cast<bson::int64_type*>(d.data()));
}

TEST(DecoderTest,Null)
{
    bson::decoder d;
    d.decode(nullptr);
    EXPECT_EQ(nullptr, *reinterpret_cast<bson::null_type*>(d.data()));
}

TEST(DecoderTest,TypeMap)
{
    bson::double_type b1;
    bson::string_type b2;
    bson::document_type b3;
    bson::array_type b4;
    bson::boolean_type b8;
    bson::date_type b9;
    bson::null_type b10;
    bson::int32_type b16;
    bson::int64_type b18;

    std::clog << bson::type(b1)  << "\n"
              << bson::type(b2) << "\n"
              << bson::type(b3)  << "\n"
              << bson::type(b4)  << "\n"
              << bson::type(b8)  << "\n"
              << bson::type(b9)  << "\n"
              << bson::type(b10) << "\n"
              << bson::type(b16) << "\n"
              << bson::type(b18) << "\n";
}
