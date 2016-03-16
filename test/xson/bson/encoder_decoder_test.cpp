#include <sstream>
#include <limits>
#include <gtest/gtest.h>
#include "xson/bson.hpp"

using namespace std;
using namespace std::string_literals;
using namespace std::chrono;
using namespace xson;
using namespace xson::bson;

TEST(XsonBsonEncoderDecoderTest,Double)
{
    stringstream ss;
    ss << bson::object{"Double", 21.12};        // encode

    auto result = bson::parse(ss);              // decode
    TRACE(result);

    ASSERT_EQ(type::number, result["Double"s].type());
    double d = result["Double"s];
    ASSERT_EQ(21.12, d);
}

TEST(XsonBsonEncoderDecoderTest,String)
{
    stringstream ss;
    ss << bson::object{"String"s,"Aku Ankka"s}; // encode

    auto result = bson::parse(ss);              // decode
    TRACE(result);

    ASSERT_EQ(type::string, result["String"s].type());
    string s = result["String"s];
    ASSERT_EQ("Aku Ankka"s, s);
}

TEST(XsonBsonEncoderDecoderTest,Array)
{
    stringstream ss;
    ss << bson::object {
        {"Inhabitants"s, std::vector<std::string>{"Mikki"s, "Hessu"s, "Roope"s, "Pelle"s}}
    };

    auto result = bson::parse(ss);
    TRACE(result);

    ASSERT_EQ(type::array, result["Inhabitants"s].type());

    ASSERT_EQ(type::string, result["Inhabitants"s][0].type());
    string i1 = result["Inhabitants"s][0];
    ASSERT_EQ("Mikki"s, i1);

    ASSERT_EQ(type::string, result["Inhabitants"s][1].type());
    string i2 = result["Inhabitants"s][1];
    ASSERT_EQ("Hessu"s, i2);

    ASSERT_EQ(type::string, result["Inhabitants"s][2].type());
    string i3 = result["Inhabitants"s][2];
    ASSERT_EQ("Roope"s, i3);

    ASSERT_EQ(type::string, result["Inhabitants"s][3].type());
    string i4 = result["Inhabitants"s][3];
    ASSERT_EQ("Pelle"s, i4);
}

TEST(XsonBsonEncoderDecoderTest,Date)
{
    const auto now = system_clock::now();
    stringstream ss;
    ss << bson::object{{"Date1"s, now}, {"Date2"s, nullptr}};

    auto result = bson::parse(ss);
    TRACE(result);

    ASSERT_EQ(type::date, result["Date1"s].type());
    string d1 = result["Date1"s];
    EXPECT_EQ(to_string(now), d1);

    ASSERT_EQ(type::null, result["Date2"s].type());
    string d2 = result["Date2"s];
    EXPECT_EQ("null"s, d2);
}

TEST(XsonBsonEncoderDecoderTest,Null)
{
    stringstream ss;
    ss << bson::object{"Null"s, nullptr};

    auto result = bson::parse(ss);
    TRACE(result);

    ASSERT_EQ(type::null, result["Null"s].type());
    string n = result["Null"s];
    ASSERT_EQ("null"s, n);
}

TEST(XsonBsonEncoderDecoderTest,Int32)
{
    stringstream ss;
    ss << bson::object{"MinInt32"s, numeric_limits<std::int32_t>::min()};

    auto result = bson::parse(ss);
    TRACE(result);

    ASSERT_EQ(type::int32, result["MinInt32"s].type());
    int i = result["MinInt32"s];
    ASSERT_EQ(numeric_limits<std::int32_t>::min(), i);
}

TEST(XsonBsonEncoderDecoderTest,Int64)
{
    stringstream ss;
    ss << bson::object{"MinInt64"s, numeric_limits<std::int64_t>::max()};

    auto result = bson::parse(ss);
    TRACE(result);

    ASSERT_EQ(type::int64, result["MinInt64"s].type());
    long long i = result["MinInt64"s];
    ASSERT_EQ(numeric_limits<std::int64_t>::max(), i);
}

TEST(XsonBsonEncoderDecoderTest,SimplebsonObject)
{
    bson::object doc
    {
        {"Ruoka",true},
        {"Onni",false},
        {"Tulppu",1},
        {"Elppu",2},
        {"Jalppu",3}
    };

    stringstream ss;
    ss << doc;

    auto result = bson::parse(ss);
    TRACE(result);

    bool b1 = result["Ruoka"s];
    ASSERT_EQ(true, b1);

    bool b2 = result["Onni"s];
    ASSERT_EQ(false, b2);
}

TEST(XsonBsonEncoderDecoderTest,NestedbsonObject)
{
    bson::object doc
    {
        { "Ruoka",  true                          },
        { "Onni",   false                         },
        { "Tulppu", 1                             },
        { "Elppu",  2                             },
        { "Jalppu", 3                             },
        { "Ages",   std::vector<int>{39,40,9,5,2} }
    };

    stringstream ss;
    ss << doc;

    auto result = bson::parse(ss);
    TRACE(result);

    int i = result["Ages"s][4];
    ASSERT_EQ(2, i);
}
