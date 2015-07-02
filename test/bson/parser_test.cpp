#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include "bson/parser.hpp"
#include "bson/model.hpp"

using namespace std::literals::string_literals;
using std::stringstream;
using bson::document;

TEST(BsonParserTest,Mockup)
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
}
