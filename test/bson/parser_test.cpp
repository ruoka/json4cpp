#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include "bson/parser.hpp"
#include "bson/model.hpp"

using namespace std::literals::string_literals;
using std::stringstream;
using std::clog;
using std::endl;
using bson::document;
using bson::array;

TEST(BsonParserTest,Simple)
{
    document doc
    {
        {"Ruoka",true},
        {"Onninen",false},
        {"Tulus",1},
        {"Elo",2},
        {"Jalo",3},
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
        {"Onninen",false},
        {"Tulus",1},
        {"Elo",2},
        {"Jalo",3},
        {"Ages",array{39,40,9,5,2}}
    };

    stringstream ios;

    ios << doc;

    auto result = bson::parse(ios);

    clog << result << endl;
}
