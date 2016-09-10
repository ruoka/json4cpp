#include <gtest/gtest.h>
#include "xson/bson/encoder.hpp"

using namespace std::string_literals;
using namespace xson;
using namespace xson::bson;

TEST(XsonBsonEncoderTest1,Int32)
{
    auto e = encoder{};
    e.encode(std::int32_t{2});
    EXPECT_EQ(2, *reinterpret_cast<const std::int32_t*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Int64)
{
    auto e = encoder{};
    e.encode(std::int64_t{3});
    EXPECT_EQ(3, *reinterpret_cast<const std::int64_t*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Double)
{
    auto e = encoder{};
    e.encode(std::double_t{4.0});
    EXPECT_EQ(4.0, *reinterpret_cast<const std::double_t*>(e.data()));
}

TEST(XsonBsonEncoderTest1,CString)
{
    auto e = encoder{};
    e.encode("Ruoka"s, true);
    EXPECT_STREQ("Ruoka"s.c_str(), e.data());
}

TEST(XsonBsonEncoderTest1,String)
{
    auto e = encoder{};
    e.encode("Ruoka"s, false);
    EXPECT_EQ("Ruoka"s.size()+1, *reinterpret_cast<const std::int32_t*>(e.data())); // bytes
    EXPECT_STREQ("Ruoka"s.c_str(), e.data()+4);                                   // data
}

TEST(XsonBsonEncoderTest1,BooleanTrue)
{
    auto e = encoder{};
    e.encode(true);
    EXPECT_EQ(true, *reinterpret_cast<const std::bool_t*>(e.data()));
}

TEST(XsonBsonEncoderTest1,BooleanFalse)
{
    auto e = encoder{};
    e.encode(false);
    EXPECT_EQ(false, *reinterpret_cast<const std::bool_t*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Date)
{
    using namespace std::chrono;
    const auto val = system_clock::now();
    const auto ref = duration_cast<milliseconds>(val.time_since_epoch()).count();
    auto e = encoder{};
    e.encode(ref);
    EXPECT_EQ(ref, *reinterpret_cast<const std::int64_t*>(e.data()));
}

TEST(XsonBsonEncoderTest1,Null)
{
    auto e = encoder{};
    e.encode(nullptr);
    EXPECT_EQ(nullptr, *reinterpret_cast<const std::nullptr_t*>(e.data()));
}
