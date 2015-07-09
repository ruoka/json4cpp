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

TEST(DecoderTest,CString)
{
    bson::decoder d;
    d.decode("Ruoka"s, true);
    EXPECT_STREQ("Ruoka"s.c_str(), d.data());
}

TEST(DecoderTest,String)
{
    bson::decoder d;
    d.decode("Ruoka"s, false);
    EXPECT_EQ("Ruoka"s.size()+1, *reinterpret_cast<bson::int32_type*>(d.data())); // bytes
    EXPECT_STREQ("Ruoka"s.c_str(), d.data()+4);                                   // data
}

TEST(DecoderTest,BooleanTrue)
{
    bson::decoder d;
    d.decode(true);
    EXPECT_EQ(true, *reinterpret_cast<bson::boolean_type*>(d.data()));
}

TEST(DecoderTest,BooleanFalse)
{
    bson::decoder d;
    d.decode(false);
    EXPECT_EQ(false, *reinterpret_cast<bson::boolean_type*>(d.data()));
}

TEST(DecoderTest,Date)
{
    using namespace std::chrono;
    auto val = system_clock::now();
    auto ref = duration_cast<milliseconds>(val.time_since_epoch()).count();
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
