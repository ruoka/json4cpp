#include <gtest/gtest.h>
#include "xson/bson/encoder.hpp"

using namespace std::string_literals;
using namespace xson;
using namespace xson::bson;

TEST(XsonBsonEncoderTest1,Int32)
{
    encoder e;
    e.encode(int32_type{2});
    EXPECT_EQ(2, *reinterpret_cast<int32_type*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Int64)
{
    encoder e;
    e.encode(int64_type{3});
    EXPECT_EQ(3, *reinterpret_cast<int64_type*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Double)
{
    encoder e;
    e.encode(double_type{4.0});
    EXPECT_EQ(4.0, *reinterpret_cast<double_type*>(e.data()));
}

TEST(XsonBsonEncoderTest1,CString)
{
    encoder e;
    e.encode("Ruoka"s, true);
    EXPECT_STREQ("Ruoka"s.c_str(), e.data());
}

TEST(XsonBsonEncoderTest1,String)
{
    encoder e;
    e.encode("Ruoka"s, false);
    EXPECT_EQ("Ruoka"s.size()+1, *reinterpret_cast<int32_type*>(e.data())); // bytes
    EXPECT_STREQ("Ruoka"s.c_str(), e.data()+4);                                   // data
}

TEST(XsonBsonEncoderTest1,BooleanTrue)
{
    encoder e;
    e.encode(true);
    EXPECT_EQ(true, *reinterpret_cast<boolean_type*>(e.data()));
}

TEST(XsonBsonEncoderTest1,BooleanFalse)
{
    encoder e;
    e.encode(false);
    EXPECT_EQ(false, *reinterpret_cast<boolean_type*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Date)
{
    using namespace std::chrono;
    const auto val = system_clock::now();
    const auto ref = duration_cast<milliseconds>(val.time_since_epoch()).count();
    encoder e;
    e.encode(ref);
    EXPECT_EQ(ref, *reinterpret_cast<int64_type*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Null)
{
    encoder e;
    e.encode(nullptr);
    EXPECT_EQ(nullptr, *reinterpret_cast<null_type*>(e.data()));
}
