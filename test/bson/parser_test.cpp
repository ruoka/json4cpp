#include <sstream>
#include <fstream>
#include <limits>
#include <gtest/gtest.h>
#include "bson/parser.hpp"
#include "bson/model.hpp"

using namespace std::literals::string_literals;
using std::stringstream;
using std::clog;
using std::endl;
using std::chrono::system_clock;
using bson::document;
using bson::array;

TEST(BsonParserTest,Double)
{
    document doc
    {
        {"Today",21.12}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,String)
{
    document doc
    {
        {"Name"s,"Aku Ankka"s}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,Array)
{
    document doc
    {
        {"Inhabitant"s, array{"Mikki"s,"Hessu"s,"Roope"s,"Pelle"s}}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,Date)
{
    document doc
    {
        {"Today",system_clock::now()}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,Null)
{
    document doc
    {
        {"Ideas",nullptr}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,Int32)
{
    document doc
    {
        {"MinInt32",std::numeric_limits<std::int32_t>::min()}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,Int64)
{
    document doc
    {
        {"MaxInt64",std::numeric_limits<std::int64_t>::max()}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}

TEST(BsonParserTest,Simple)
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

    clog << result << endl;
}

TEST(BsonParserTest,Nested)
{
    document doc
    {
        {"Ruoka",true},
        {"Onni",false},
        {"Tulppu",1},
        {"Elppu",2},
        {"Jalppu",3},
        {"Ages",array{39,40,9,5,2}}
    };

    stringstream ios;
    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}
