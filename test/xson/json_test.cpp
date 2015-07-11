#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace xson;
using namespace xson::json;

TEST(XsonJsonTest,String)
{
    stringstream ss;
    ss << json::object{"String", "1234567890"s};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    std::string s = ob["String"s];
    ASSERT_EQ("1234567890"s, s);
    ASSERT_EQ(type::string, ob["String"s].type());
}

TEST(XsonJsonTest,Double)
{
    stringstream ss;
    ss << json::object{"Double", 21.12};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    double d = ob["Double"s];
    ASSERT_EQ(21.12, d);
    ASSERT_EQ(type::number, ob["Double"s].type());
}

TEST(XsonJsonTest,Boolean)
{
    stringstream ss;
    ss << json::object{{"True", true},{"False", false}};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    bool t = ob["True"s];
    ASSERT_EQ(true, t);
    ASSERT_EQ(type::boolean, ob["True"s].type());

    bool f = ob["False"s];
    ASSERT_EQ(false, f);
    ASSERT_EQ(type::boolean, ob["False"s].type());
}

TEST(XsonJsonTest,Date2String)
{
    auto now = std::chrono::system_clock::now();
    stringstream ss;
    ss << json::object{"Date", now};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    std::string d = ob["Date"s];
    ASSERT_EQ(std::to_string(now), d);
//  EXPECT_EQ(type::date, ob["Date"s].type());
    ASSERT_EQ(type::string, ob["Date"s].type());
}

TEST(XsonJsonTest,Null)
{
    stringstream ss;
    ss << json::object{"Null", nullptr};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    std::string n = ob["Null"s];
    ASSERT_EQ("null"s, n);
    ASSERT_EQ(type::null, ob["Null"s].type());
}

TEST(XsonJsonTest,Int32)
{
    stringstream ss;
    ss << json::object{
        {"Zero", std::int32_t{0}},
        {"Min", std::numeric_limits<int>::min()},
        {"Max", std::numeric_limits<int>::max()}
    };
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    int zero = ob["Zero"s];
    ASSERT_EQ(0, zero);
    ASSERT_EQ(type::int32, ob["Zero"s].type());

    int min = ob["Min"s];
    ASSERT_EQ(std::numeric_limits<int>::min(), min);
    ASSERT_EQ(type::int32, ob["Min"s].type());

    int max = ob["Max"s];
    ASSERT_EQ(std::numeric_limits<int>::max(), max);
    ASSERT_EQ(type::int32, ob["Max"s].type());
}

TEST(XsonJsonTest,Int64)
{
    stringstream ss;
    ss << json::object{
        {"Zero", std::int64_t{0}},
        {"Min", std::numeric_limits<long long>::min()},
        {"Max", std::numeric_limits<long long>::max()}
    };
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    long long zero = ob["Zero"s];
    ASSERT_EQ(0, zero);
//  EXPECT_EQ(type::int64, ob["Zero"s].type());
    ASSERT_EQ(type::int32, ob["Zero"s].type());

    long long min = ob["Min"s];
    ASSERT_EQ(std::numeric_limits<long long>::min(), min);
    ASSERT_EQ(type::int64, ob["Min"s].type());

    long long max = ob["Max"s];
    ASSERT_EQ(std::numeric_limits<long long>::max(), max);
    ASSERT_EQ(type::int64, ob["Max"s].type());
}

TEST(XsonJsonTest,Array)
{
    auto ob1 = json::object
    {
        {"Array"s, std::array<int,3>{1, 2, 3}}
    };
    TRACE("ob1:  " << ob1);
    auto str1 = json::stringify(ob1);
    TRACE("str1: " << str1);

    auto ss = std::stringstream{str1};

    auto ob2 = json::parse(ss);
    TRACE("ob2:  " << ob2);
    auto str2 = json::stringify(ob1);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonJsonTest,Vector)
{
    auto ob1 = json::object
    {
        {"Vector"s, std::vector<std::string>{"a", "b", "c", "d", "e", "f"}}
    };
    TRACE("ob1:  " << ob1);
    auto str1 = json::stringify(ob1);
    TRACE("str1: " << str1);

    auto ss = std::stringstream{str1};

    auto ob2 = json::parse(ss);
    TRACE("ob2:  " << ob2);
    auto str2 = json::stringify(ob1);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonJsonTest,ParseFile)
{
    using namespace std::literals::string_literals;
    using namespace xson::json;

    ifstream fs{"./test/json/test2.json"};
    auto ob = json::parse(fs);
    TRACE("test2.json: "s << ob);

    const bool alive = ob["isAlive"s];
    EXPECT_EQ(true, alive);
    ASSERT_EQ(type::boolean, ob["isAlive"s].type());

    const std::string spouce = ob["spouse"s];
    EXPECT_EQ("null"s, spouce);
    ASSERT_EQ(type::null, ob["spouse"s].type());
}

TEST(XsonJsonTest,Complex)
{
    xson::object mix
    {
        { "Ruoka",  true                          },
        { "Onni",   false                         },
        { "Tulppu", 1                             },
        { "Elppu",  2                             },
        { "Jalppu", 3                             },
        { "Ages",   std::vector<int>{39,40,9,5,2} }
    };

    std::stringstream ss;
    ss << mix;
    TRACE(ss.str());

    auto obj = json::parse(ss);
    TRACE(obj);

    ASSERT_EQ(type::boolean, obj["Ruoka"s].type());
    const bool b = obj["Ruoka"s];
    ASSERT_EQ(true, b);

    ASSERT_EQ(type::int32, obj["Tulppu"s].type());
    const int i1 = obj["Tulppu"s];
    ASSERT_EQ(1, i1);

    ASSERT_EQ(type::array, obj["Ages"s].type());

    ASSERT_EQ(type::int32, obj["Ages"s][4].type());
    const int i2 = obj["Ages"s][4];
    ASSERT_EQ(2, i2);
}
