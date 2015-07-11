#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace chrono;
using namespace xson;
using namespace json;

TEST(XsonJsonTest,String)
{
    stringstream ss;
    ss << json::object{"String", "1234567890"s};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::string, ob["String"s].type());
    string s = ob["String"s];
    ASSERT_EQ("1234567890"s, s);
}

TEST(XsonJsonTest,Double)
{
    stringstream ss;
    ss << json::object{"Double", 21.12};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::number, ob["Double"s].type());
    double d = ob["Double"s];
    ASSERT_EQ(21.12, d);
}

TEST(XsonJsonTest,Boolean)
{
    stringstream ss;
    ss << json::object{{"True", true},{"False", false}};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::boolean, ob["True"s].type());
    bool t = ob["True"s];
    ASSERT_EQ(true, t);

    ASSERT_EQ(type::boolean, ob["False"s].type());
    bool f = ob["False"s];
    ASSERT_EQ(false, f);
}

TEST(XsonJsonTest,Date2String)
{
    auto now = system_clock::now();
    stringstream ss;
    ss << json::object{"Date", now};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::string, ob["Date"s].type());
    string d = ob["Date"s];
    ASSERT_EQ(to_string(now), d);
}

TEST(XsonJsonTest,Null)
{
    stringstream ss;
    ss << json::object{"Null", nullptr};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::null, ob["Null"s].type());
    string n = ob["Null"s];
    ASSERT_EQ("null"s, n);
}

TEST(XsonJsonTest,Int32)
{
    stringstream ss;
    ss << json::object{
        {"Zero", int32_t{0}},
        {"Min", numeric_limits<int>::min()},
        {"Max", numeric_limits<int>::max()}
    };
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::int32, ob["Zero"s].type());
    int zero = ob["Zero"s];
    ASSERT_EQ(0, zero);

    ASSERT_EQ(type::int32, ob["Min"s].type());
    int min = ob["Min"s];
    ASSERT_EQ(numeric_limits<int>::min(), min);

    ASSERT_EQ(type::int32, ob["Max"s].type());
    int max = ob["Max"s];
    ASSERT_EQ(numeric_limits<int>::max(), max);
}

TEST(XsonJsonTest,Int64)
{
    stringstream ss;
    ss << json::object{
        {"Zero", int64_t{0}},
        {"Min", numeric_limits<long long>::min()},
        {"Max", numeric_limits<long long>::max()}
    };
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::int32, ob["Zero"s].type());
    long long zero = ob["Zero"s];
    ASSERT_EQ(0, zero);

    ASSERT_EQ(type::int64, ob["Min"s].type());
    long long min = ob["Min"s];
    ASSERT_EQ(numeric_limits<long long>::min(), min);

    ASSERT_EQ(type::int64, ob["Max"s].type());
    long long max = ob["Max"s];
    ASSERT_EQ(numeric_limits<long long>::max(), max);
}

TEST(XsonJsonTest,Array)
{
    auto ob1 = json::object
    {
        {"Array"s, array<int,3>{1, 2, 3}}
    };
    TRACE("ob1:  " << ob1);
    auto str1 = json::stringify(ob1);
    TRACE("str1: " << str1);

    auto ss = stringstream{str1};

    auto ob2 = json::parse(ss);
    TRACE("ob2:  " << ob2);
    auto str2 = json::stringify(ob2);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonJsonTest,Vector)
{
    auto ob1 = json::object
    {
        {"Vector"s, vector<string>{"a", "b", "c", "d", "e", "f"}}
    };
    TRACE("ob1:  " << ob1);
    auto str1 = json::stringify(ob1);
    TRACE("str1: " << str1);

    auto ss = stringstream{str1};

    auto ob2 = json::parse(ss);
    TRACE("ob2:  " << ob2);
    auto str2 = json::stringify(ob2);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonJsonTest,Complex)
{
    xson::object mix
    {
        { "Ruoka",  true                     },
        { "Onni",   false                    },
        { "Tulppu", 1                        },
        { "Elppu",  2                        },
        { "Jalppu", 3                        },
        { "Ages",   vector<int>{39,40,9,5,2} }
    };

    stringstream ss;
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

TEST(XsonJsonTest,ParseFile1)
{
    using namespace string_literals;
    using namespace xson::json;

    ifstream fs{"./test/xson/test1.json"};
    auto ob = json::parse(fs);
    TRACE("test1.json: "s << ob);

    ASSERT_EQ(type::array, ob["required"s].type());
    ASSERT_EQ(type::string, ob["required"s][2].type());
    const string price = ob["required"s][2];
    EXPECT_EQ("price"s, price);

    ASSERT_EQ(type::object, ob["properties"s].type());
    ASSERT_EQ(type::object, ob["properties"s]["price"s].type());
    ASSERT_EQ(type::int32, ob["properties"s]["price"s]["minimum"s].type());
    const int minimum = ob["properties"s]["price"s]["minimum"s];
    EXPECT_EQ(0, minimum);
}

TEST(XsonJsonTest,ParseFile2)
{
    using namespace string_literals;
    using namespace xson::json;

    ifstream fs{"./test/xson/test2.json"};
    auto ob = json::parse(fs);
    TRACE("test2.json: "s << ob);

    const bool alive = ob["isAlive"s];
    EXPECT_EQ(true, alive);
    ASSERT_EQ(type::boolean, ob["isAlive"s].type());

    const string spouce = ob["spouse"s];
    EXPECT_EQ("null"s, spouce);
    ASSERT_EQ(type::null, ob["spouse"s].type());
}
