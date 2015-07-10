#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace xson;
using namespace xson::json;

TEST(XsonJsonTest,Stringify)
{
    auto ob1 = json::object
    {
        {"Test"s, std::array<int,3>{1, 2, 3}}
    };

    TRACE(ob1);

    auto str = json::stringify(ob1);

    auto ss = std::stringstream{str};

    auto ob2 = json::parse(ss);

    TRACE(ob2);
}

TEST(XsonJsonTest,Double)
{
    stringstream ss;
    ss << json::object{"Double", 21.12};

    auto ob = json::parse(ss);
    TRACE(ob);

    double d = ob["Double"s];
    ASSERT_EQ(d, 21.12);
}

TEST(XsonJsonTest,ParseFile)
{
    using namespace std::literals::string_literals;
    using namespace xson::json;

    ifstream fs{"./test/json/test2.json"};
    auto ob = json::parse(fs);
    clog << "test2.json: "s << ob << endl;

    bool alive = ob["isAlive"s];
    EXPECT_EQ(alive,true);

    bool spouce = ob["spouse"s];
    EXPECT_EQ(spouce,false);
}
