#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include "json/parser.hpp"

using namespace std;
using namespace literals::string_literals;

TEST(JsonParserTest,Mockup)
{
    stringstream ios;
    ios << "{\"_id\":2,\"Name\":\"Ruoka\", \"Embedded\":{\"_id\":5,\"Name\":\"Tuma\"}, \"Lucky Numbers\":[2,22,2112] }" << endl;

    clog << ios.str() << endl;

    auto result = json::parse(ios);

    clog << result << endl;

    clog << "_id = "  << result["_id"s]                           << "\n"
         << "Name = " << result["Name"s]                          << "\n"
         << "Embeded.Name = "   << result["Embedded"s]["Name"s]   << "\n"
         << "Lucky Number 1 = " << result["Lucky Numbers"s][0]    << "\n"
         << "Lucky Number 2 = " << result["Lucky Numbers"s][1]    << "\n"
         << "Lucky Number 3 = " << result["Lucky Numbers"s]["2"s] << endl;

    int id = result["_id"s];
    ASSERT_EQ(id,2);

    string name = result["Name"s];
    ASSERT_EQ(name,"Ruoka"s);

    double number = result["Lucky Numbers"s][1];
    ASSERT_EQ(number,double{22});
}

TEST(JsonParserTest,ParseTest1)
{
    ifstream ifs{"./test/json/test1.json"};
    auto result = json::parse(ifs);
    clog << "test1.json: "s << result << endl;
}

TEST(JsonParserTest,ParseTest2)
{
    ifstream ifs{"./test/json/test2.json"};
    auto result = json::parse(ifs);
    clog << "test2.json: "s << result << endl;

    bool alive = result["isAlive"s];
    EXPECT_EQ(alive,true);

//    bool spouce = result["spouse"s]; // FIXME
//    EXPECT_EQ(spouce,false);
}
