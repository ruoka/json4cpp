#include <iostream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std::literals::string_literals;
using namespace xson;
using namespace xson::json;
using string = std::string;
using nullptr_t = std::nullptr_t;
using date = std::chrono::system_clock::time_point;
using array = std::array<int,9>;
using vector = std::vector<string>;

TEST(XsonObjectTest,IsValue)
{
    ASSERT_EQ(xson::is_value<int>(),       true);
    ASSERT_EQ(xson::is_value<long>(),      true);
    ASSERT_EQ(xson::is_value<double>(),    true);
    ASSERT_EQ(xson::is_value<bool>(),      true);
    ASSERT_EQ(xson::is_value<string>(),    true);
    ASSERT_EQ(xson::is_value<date>(),      true);
    ASSERT_EQ(xson::is_value<nullptr_t>(), true);
    ASSERT_EQ(xson::is_value<object>(),    false);
    ASSERT_EQ(xson::is_value<array>(),     false);
    ASSERT_EQ(xson::is_value<vector>(),    false);
    ASSERT_EQ(xson::is_value<int[99]>(),   false);
}

TEST(XsonObjectTest,IsArray)
{
    ASSERT_EQ(xson::is_array<int>(),       false);
    ASSERT_EQ(xson::is_array<long>(),      false);
    ASSERT_EQ(xson::is_array<double>(),    false);
    ASSERT_EQ(xson::is_array<bool>(),      false);
    ASSERT_EQ(xson::is_array<string>(),    false);
    ASSERT_EQ(xson::is_array<date>(),      false);
    ASSERT_EQ(xson::is_array<nullptr_t>(), false);
    ASSERT_EQ(xson::is_array<object>(),    false);
    ASSERT_EQ(xson::is_array<array>(),     true);
    ASSERT_EQ(xson::is_array<vector>(),    true);
    ASSERT_EQ(xson::is_array<int[99]>(),   true);
}

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
    int idx{0};
    for(const auto& a : arr)
    {
        const int i = o["Array"s][idx++];
        ASSERT_EQ(i, a);
    }
}

TEST(XsonObjectTest,Vector)
{
    vector vec = {"a","b","c","d","e","f","g","h","i"};
    xson::object o{"Vector"s, vec};
    TRACE(o);
    int idx{0};
    for(const auto& v : vec)
    {
        const string s = o["Vector"s][idx++];
        ASSERT_EQ(s, v);
    }
}

TEST(XsonObjectTest,CArray)
{
    double arr[] = {1.0, 1.1, 1.12, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
    xson::object o{"CArray"s, arr};
    TRACE(o);
    int idx{0};
    for(const auto& a : arr)
    {
        const double d = o["CArray"s][idx++];
        ASSERT_EQ(d, a);
    }
}

TEST(XsonObjectTest,BooleanTrue)
{
    auto o = xson::object{"True"s, true};
    TRACE(o);
    const string s = o["True"s];
    ASSERT_EQ(s, "true"s);
}

TEST(XsonObjectTest,BooleanFalse)
{
    auto o = xson::object{"False"s, false};
    TRACE(o);
    const string s = o["False"s];
    ASSERT_EQ(s, "false"s);
}

TEST(XsonObjectTest,Null)
{
    auto o = xson::object{"Null"s, nullptr};
    TRACE(o);
    const string s = o["Null"s];
    ASSERT_EQ(s, "null"s);
}

TEST(XsonObjectTest,Date)
{
    auto now = std::chrono::system_clock::now();
    auto o = xson::object{"Date"s, now};
    TRACE(o);
    const string s = o["Date"s];
    ASSERT_EQ(s, std::to_string(now));
}

TEST(XsonObjectTest,Complex)
{
    xson::object doc
    {
        { "Ruoka",  true                          },
        { "Onni",   false                         },
        { "Tulppu", 1                             },
        { "Elppu",  2                             },
        { "Jalppu", 3                             },
        { "Ages",   std::vector<int>{39,40,9,5,2} }
    };
    TRACE(doc);

    const int i1 = doc["Tulppu"s];
    ASSERT_EQ(i1, 1);

    const int i2 = doc["Ages"s][4];
    ASSERT_EQ(i2, 2);

    std::stringstream ios;
    ios << doc;

    auto obj = json::parse(ios);
    TRACE(obj);

    const int i3 = doc["Tulppu"s];
    ASSERT_EQ(i3, 1);

    const int i4 = doc["Ages"s][4];
    ASSERT_EQ(i4, 2);
}
