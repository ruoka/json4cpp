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

TEST(XsonObjectTest,IsValue)
{
    std::cout << xson::is_value<int>::value << std::endl;
    std::cout << xson::is_value<string>::value << std::endl;
    std::cout << xson::is_value<array>::value << std::endl;
    std::cout << xson::is_value<vector>::value << std::endl;
}

TEST(XsonObjectTest,IsArray)
{
    std::cout << xson::is_array<int>::value << std::endl;
    std::cout << xson::is_array<string>::value << std::endl;
    std::cout << xson::is_array<array>::value << std::endl;
    std::cout << xson::is_array<vector>::value << std::endl;
}

TEST(XsonObjectTest,Create)
{
    auto o1 = xson::object{"Integer"s, 123456789};
    std::clog << o1 << std::endl;

    xson::object o2 =
    {
        {"Integer"s, 987654321},
        {"Double"s, 21.12},
        {"Object1"s, o1},
        {"Object2"s, xson::object{{"Boolean"s, false}, {"String"s, "4"s}}},
    };
    std::clog << o2 << std::endl;

    array arr = {1,2,3,4,5,6,7,8,9};
    xson::object o3{"Array"s, arr};
    std::clog << o3 << std::endl;

    vector vec = {"a","b","c","d","e","f","g","h","i"};
    xson::object o4{"Vector"s, vec};
    std::clog << o4 << std::endl;

    auto o5 = xson::object{"Null"s, nullptr};
    std::clog << o5 << std::endl;

    auto o6 = xson::object{"Time"s, std::chrono::system_clock::now()};
    std::clog << o6 << std::endl;
}

TEST(XsonObjectTest,NestedDocument)
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

    int i0 = doc["Tulppu"s];
    ASSERT_EQ(i0, 1);

    int i1 = doc["Ages"s][4];
    ASSERT_EQ(i1, 2);

    std::stringstream ios;
    ios << doc;

    auto obj = json::parse(ios);
//    json::object obj;
//    ios >> obj;
    TRACE(obj);

    int i2 = obj["Ages"s][4];
    ASSERT_EQ(i2, 2);
}
