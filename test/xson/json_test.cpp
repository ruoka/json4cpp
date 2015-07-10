#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"

using namespace std;
using namespace xson;

/*
TEST(XsonJsonTest,Parse)
{
    auto& is = std::cin;
    auto object = json::parse(is);
}

TEST(XsonJsonTest,Stringify)
{
    auto obj = json::object{};
    auto str = json::stringify(obj);
}
*/

TEST(XsonJsonTest,ParseFile)
{
    using namespace std::literals::string_literals;
    using namespace xson::json;

    ifstream ifs{"./test/json/test2.json"};
    auto obj = json::parse(ifs);
    clog << "test2.json: "s << obj << endl;

    bool alive = obj["isAlive"s];
    EXPECT_EQ(alive,true);

    bool spouce = obj["spouse"s];
    EXPECT_EQ(spouce,false);
}
