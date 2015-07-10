#include <fstream>
#include <gtest/gtest.h>
#include "xson/bson.hpp"

using namespace std;
using namespace xson;
using namespace xson::bson;

TEST(XsonBsonTest,Stringify)
{
    auto ob1 = bson::object
    {
        {"Test"s, std::array<int,3>{1, 2, 3}}
    };

    TRACE(ob1);

    auto str = bson::stringify(ob1);

    auto ss = std::stringstream{str};

    auto ob2 = bson::parse(ss);

    TRACE(ob2);
}

TEST(XsonBsonTest,Double)
{
    stringstream ss;
    ss << bson::object{"Double", 21.12};

    auto ob = bson::parse(ss);
    TRACE(ob);

    double d = ob["Double"s];
    ASSERT_EQ(d, 21.12);
}
