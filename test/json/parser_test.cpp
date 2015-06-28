#include <string>
#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "json/parser.hpp"

using namespace std::literals::string_literals;

TEST(JsonParserTest,Mockup)
{
    std::stringstream ios;
    ios << "{\"_id\":2,\"Name\":\"Ruoka\", \"Embeded\":{\"_id\":5,\"Name\":\"Tuma\"}, \"Lucky Numbers\":[2,22,2112] }" << std::endl;

    std::clog << ios.str() << std::endl;

    auto result = json::parse(ios);

    std::clog << result << std::endl;

    std::clog << "_id = "  << result["_id"s]  << "\n"
              << "name = " << result["Name"s] << "\n"
              << "name = " << result["Embeded"s]["Name"s]  << "\n"
              << "number 1 = " << result["Lucky Numbers"s][0]  << "\n"
              << "number 2 = " << result["Lucky Numbers"s]["2"s]  << "\n" << std::endl;

    long id = result["_id"s];
    ASSERT_EQ(id,2);

    std::string name = result["Name"s];
    ASSERT_EQ(name,"Ruoka"s);

    double number = result["Lucky Numbers"s][1];
    ASSERT_EQ(number,double{22});
}
