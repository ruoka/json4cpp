#include <sstream>
#include <fstream>
#include <limits>
#include <gtest/gtest.h>
#include "bson/parser.hpp"
#include "bson/model.hpp"
#include "std/trace.hpp"

using namespace std;
using namespace std::literals::string_literals;
using namespace std::chrono;
using namespace bson;

TEST(BsonParserTest,Double)
{
    stringstream ios;
    ios << document{"Double", 21.12};

    auto result = bson::parse(ios);
    TRACE(result);

    double d = result["Double"s];
    ASSERT_EQ(d, 21.12);
}

TEST(BsonParserTest,String)
{
    stringstream ios;
    ios << document{"String"s,"Aku Ankka"s};

    auto result = bson::parse(ios);
    TRACE(result);

    string s = result["String"s];
    ASSERT_EQ(s, "Aku Ankka"s);
}

TEST(BsonParserTest,Array)
{
    stringstream ios;
    ios << document {
        {"Inhabitants"s, bson::array{"Mikki"s, "Hessu"s, "Roope"s, "Pelle"s}}
    };

    auto result = bson::parse(ios);
    TRACE(result);

    string i1 = result["Inhabitants"s][0];
    ASSERT_EQ(i1, "Mikki"s);

    string i2 = result["Inhabitants"s][1];
    ASSERT_EQ(i2, "Hessu"s);

    string i3 = result["Inhabitants"s][2];
    ASSERT_EQ(i3, "Roope"s);

    string i4 = result["Inhabitants"s][3];
    ASSERT_EQ(i4, "Pelle"s);
}

TEST(BsonParserTest,Date)
{
    const auto now = system_clock::now();
    stringstream ios;
    ios << document{{"Date1"s, now}, {"Date2"s, nullptr}};

    auto result = bson::parse(ios);
    TRACE(result);

    string d1 = result["Date1"s];
    EXPECT_EQ(d1, to_string(now));

    string d2 = result["Date2"s];
    EXPECT_EQ(d2, "null"s);
}

TEST(BsonParserTest,Null)
{
    stringstream ios;
    ios << document{"Null"s, nullptr};

    auto result = bson::parse(ios);
    TRACE(result);

    string n = result["Null"s];
    ASSERT_EQ(n, "null"s);
}

TEST(BsonParserTest,Int32)
{
    stringstream ios;
    ios << document{"MinInt32"s, numeric_limits<std::int32_t>::min()};

    auto result = bson::parse(ios);
    TRACE(result);

    int i = result["MinInt32"s];
    ASSERT_EQ(i, numeric_limits<std::int32_t>::min());
}

TEST(BsonParserTest,Int64)
{
    stringstream ios;
    ios << document{"MinInt64"s, numeric_limits<std::int64_t>::max()};

    auto result = bson::parse(ios);
    TRACE(result);

    long i = result["MinInt64"s];
    ASSERT_EQ(i, numeric_limits<std::int64_t>::max());
}

TEST(BsonParserTest,SimpleDocument)
{
    document doc
    {
        {"Ruoka",true},
        {"Onni",false},
        {"Tulppu",1},
        {"Elppu",2},
        {"Jalppu",3}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);
    TRACE(result);

    bool b1 = result["Ruoka"s];
    ASSERT_EQ(b1, true);

    bool b2 = result["Onni"s];
    ASSERT_EQ(b2, false);
}

TEST(BsonParserTest,NestedDocument)
{
    document doc
    {
        { "Ruoka",  true                     },
        { "Onni",   false                    },
        { "Tulppu", 1                        },
        { "Elppu",  2                        },
        { "Jalppu", 3                        },
        { "Ages",   bson::array{39,40,9,5,2} }
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);
    TRACE(result);

    int i = result["Ages"s][4];
    ASSERT_EQ(i, 2);
}
