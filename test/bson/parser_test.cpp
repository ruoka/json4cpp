#include <sstream>
#include <fstream>
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
