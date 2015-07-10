#include <fstream>
#include <gtest/gtest.h>
#include "xson/bson.hpp"

using namespace std;
using namespace xson;

TEST(XsonBsonTest,Parse)
{
    auto& is = std::cin;
//    auto object = bson::parse(is);
}

TEST(XsonBsonTest,Stringify)
{
    auto obj = bson::object{};
    auto str = bson::stringify(obj);
}
