#include <iostream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std::literals::string_literals;
using namespace xson;
using namespace xson::json;

using string = std::string;
using array = std::array<int,9>;
using vector = std::vector<string>;

TEST(XsonObjectTest,Mix)
{
    auto o1 = xson::object{"Integer"s, 123456789};
    TRACE(o1);
    const int i1 = o1["Integer"s];
    ASSERT_EQ(i1, 123456789);

    xson::object o2 =
    {
        {"Integer"s, 987654321},
        {"Double"s, 21.12},
        {"Object1"s, o1},
        {"Object2"s, xson::object{{"Boolean"s, false}, {"String"s, "4"s}}},
    };
    TRACE(o2);

    const int i2 = o2["Object1"s]["Integer"s];
    ASSERT_EQ(i2, 123456789);

    const double d = o2["Double"s];
    ASSERT_EQ(d, 21.12);

    string s = o2["Object2"s]["String"s];
    ASSERT_EQ(s, "4"s);
}

TEST(XsonObjectTest,Array)
{
    array arr = {1,2,3,4,5,6,7,8,9};
    xson::object o{"Array"s, arr};
    TRACE(o);
    ASSERT_EQ(type::array, o["Array"s].type());
    int idx{0};
    for(const auto& a : arr)
    {
        const int i = o["Array"s][idx];
        ASSERT_EQ(i, a);
        ASSERT_EQ(type::int32, o["Array"s][idx].type());
        ++idx;
    }
}

TEST(XsonObjectTest,Vector)
{
    vector vec = {"a","b","c","d","e","f","g","h","i"};
    xson::object o{"Vector"s, vec};
    TRACE(o);
    ASSERT_EQ(type::array, o["Vector"s].type());
    int idx{0};
    for(const auto& v : vec)
    {
        const string s = o["Vector"s][idx];
        ASSERT_EQ(v, s);
        ASSERT_EQ(type::string, o["Vector"s][idx].type());
        ++idx;
    }
}

TEST(XsonObjectTest,CArray)
{
    double arr[] = {1.0, 1.1, 1.12, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
    xson::object o{"CArray"s, arr};
    TRACE(o);
    ASSERT_EQ(type::array, o["CArray"s].type());
    int idx{0};
    for(const auto& a : arr)
    {
        const double d = o["CArray"s][idx];
        ASSERT_EQ(a, d);
        ASSERT_EQ(type::number, o["CArray"s][idx].type());
        ++idx;
    }
}

TEST(XsonObjectTest,BooleanTrue)
{
    auto o = xson::object{"True"s, true};
    TRACE(o);
    const string s = o["True"s];
    ASSERT_EQ("true"s, s);
    const bool b = o["True"s];
    ASSERT_EQ(true, b);
    ASSERT_EQ(type::boolean, o["True"s].type());
}

TEST(XsonObjectTest,BooleanFalse)
{
    auto o = xson::object{"False"s, false};
    TRACE(o);
    const string s = o["False"s];
    ASSERT_EQ("false"s, s);
    const bool b = o["False"s];
    ASSERT_EQ(false, b);
    ASSERT_EQ(type::boolean, o["False"s].type());
}

TEST(XsonObjectTest,Null)
{
    auto o = xson::object{"Null"s, nullptr};
    TRACE(o);
    const string s = o["Null"s];
    ASSERT_EQ("null"s, s);
    ASSERT_EQ(type::null, o["Null"s].type());
}

TEST(XsonObjectTest,Date)
{
    auto now = std::chrono::system_clock::now();
    auto o = xson::object{"Date"s, now};
    TRACE(o);
    const string s = o["Date"s];
    ASSERT_EQ(std::to_string(now), s);
    ASSERT_EQ(type::date, o["Date"s].type());
}

TEST(XsonObjectTest,Complex)
{
    xson::object obj
    {
        { "Ruoka",  true                          },
        { "Onni",   false                         },
        { "Tulppu", 1                             },
        { "Elppu",  2                             },
        { "Jalppu", 3                             },
        { "Ages",   std::vector<int>{39,40,9,5,2} }
    };
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
