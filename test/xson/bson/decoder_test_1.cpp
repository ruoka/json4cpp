#include <gtest/gtest.h>
#include "xson/bson/decoder.hpp"

using namespace std::string_literals;
using namespace xson;
using namespace xson::bson;

TEST(DecoderTest,Int32)
{
    decoder d;
    d.decode(int32_type{2});
    EXPECT_EQ(2, *reinterpret_cast<int32_type*>(d.data()));
}

TEST(DecoderTest,Int64)
{
    decoder d;
    d.decode(int64_type{3});
    EXPECT_EQ(3, *reinterpret_cast<int64_type*>(d.data()));
}

TEST(DecoderTest,Double)
{
    decoder d;
    d.decode(double_type{4.0});
    EXPECT_EQ(4.0, *reinterpret_cast<double_type*>(d.data()));
}

TEST(DecoderTest,CString)
{
    decoder d;
    d.decode("Ruoka"s, true);
    EXPECT_STREQ("Ruoka"s.c_str(), d.data());
}

TEST(DecoderTest,String)
{
    decoder d;
    d.decode("Ruoka"s, false);
    EXPECT_EQ("Ruoka"s.size()+1, *reinterpret_cast<int32_type*>(d.data())); // bytes
    EXPECT_STREQ("Ruoka"s.c_str(), d.data()+4);                                   // data
}

TEST(DecoderTest,BooleanTrue)
{
    decoder d;
    d.decode(true);
    EXPECT_EQ(true, *reinterpret_cast<boolean_type*>(d.data()));
}

TEST(DecoderTest,BooleanFalse)
{
    decoder d;
    d.decode(false);
    EXPECT_EQ(false, *reinterpret_cast<boolean_type*>(d.data()));
}

TEST(DecoderTest,Date)
{
    using namespace std::chrono;
    const auto val = system_clock::now();
    const auto ref = duration_cast<milliseconds>(val.time_since_epoch()).count();
    decoder d;
    d.decode(val);
    EXPECT_EQ(ref, *reinterpret_cast<int64_type*>(d.data()));
}

TEST(DecoderTest,Null)
{
    decoder d;
    d.decode(nullptr);
    EXPECT_EQ(nullptr, *reinterpret_cast<null_type*>(d.data()));
}
