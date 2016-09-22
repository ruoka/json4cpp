#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std::chrono;
using namespace xson;
using namespace json;

TEST(XsonJsonTest,String)
{
    auto ss = std::stringstream{};
    ss << json::object{"String", "1234567890"s};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::string, ob["String"s].type());
    const xson::string_type s = ob["String"s];
    ASSERT_EQ("1234567890"s, s);
}

TEST(XsonJsonTest,Double)
{
    auto ss = std::stringstream{};
    ss << json::object{"Double", 21.12};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::number, ob["Double"s].type());
    const xson::number_type d = ob["Double"s];
    ASSERT_EQ(21.12, d);
}

TEST(XsonJsonTest,Boolean)
{
    auto ss = std::stringstream{};
    ss << json::object{{"True", true},{"False", false}};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::boolean, ob["True"s].type());
    const xson::boolean_type t = ob["True"s];
    ASSERT_EQ(true, t);

    ASSERT_EQ(type::boolean, ob["False"s].type());
    const xson::boolean_type f = ob["False"s];
    ASSERT_EQ(false, f);
}

TEST(XsonJsonTest,Date2String)
{
    auto now = system_clock::now();
    auto ss = std::stringstream{};
    ss << json::object{"Date", now};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::string, ob["Date"s].type());
    const xson::string_type d = ob["Date"s];
    ASSERT_EQ(to_string(now), d);
}

TEST(XsonJsonTest,Null)
{
    auto ss = std::stringstream{};
    ss << json::object{"Null", nullptr};
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::null, ob["Null"s].type());
    const xson::null_type n = ob["Null"s];
    ASSERT_EQ(nullptr, n);
}

TEST(XsonJsonTest,Int32)
{
    auto ss = std::stringstream{};
    ss << json::object{
        {"Zero", int32_t{0}},
        {"Min", std::numeric_limits<int>::min()},
        {"Max", std::numeric_limits<int>::max()}
    };
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::int64, ob["Zero"s].type());
    const xson::integer_type zero = ob["Zero"s];
    ASSERT_EQ(0, zero);

    ASSERT_EQ(type::int64, ob["Min"s].type());
    const xson::integer_type min = ob["Min"s];
    ASSERT_EQ(std::numeric_limits<int>::min(), min);

    ASSERT_EQ(type::int64, ob["Max"s].type());
    const xson::integer_type max = ob["Max"s];
    ASSERT_EQ(std::numeric_limits<int>::max(), max);
}

TEST(XsonJsonTest,Integer)
{
    auto ss = std::stringstream{};
    ss << json::object{
        {"Zero", int64_t{0}},
        {"Min", std::numeric_limits<xson::integer_type>::min()},
        {"Max", std::numeric_limits<xson::integer_type>::max()}
    };
    TRACE(ss.str());

    auto ob = json::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::int64, ob["Zero"s].type());
    const xson::integer_type zero = ob["Zero"s];
    ASSERT_EQ(0, zero);

    ASSERT_EQ(type::int64, ob["Min"s].type());
    const xson::integer_type min = ob["Min"s];
    ASSERT_EQ(std::numeric_limits<long long>::min(), min);

    ASSERT_EQ(type::int64, ob["Max"s].type());
    const xson::integer_type max = ob["Max"s];
    ASSERT_EQ(std::numeric_limits<long long>::max(), max);
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
    auto str2 = json::stringify(ob2);
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
    auto str2 = json::stringify(ob2);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonJsonTest,Complex)
{
    auto mix = xson::object
        {
        { "Ruoka",  true                     },
        { "Onni",   false                    },
        { "Tulppu", 1                        },
        { "Elppu",  2                        },
        { "Jalppu", 3                        },
        { "Ages",   std::vector<int>{39,40,9,5,2} }
    };

    auto ss = std::stringstream{};
    ss << mix;
    TRACE(ss.str());

    auto obj = json::parse(ss);
    TRACE(obj);

    ASSERT_EQ(type::boolean, obj["Ruoka"s].type());
    const xson::boolean_type b = obj["Ruoka"s];
    ASSERT_EQ(true, b);

    ASSERT_EQ(type::int64, obj["Tulppu"s].type());
    const xson::integer_type i1 = obj["Tulppu"s];
    ASSERT_EQ(1, i1);

    ASSERT_EQ(type::array, obj["Ages"s].type());

    ASSERT_EQ(type::int64, obj["Ages"s][4].type());
    const xson::integer_type i2 = obj["Ages"s][4];
    ASSERT_EQ(2, i2);
}

TEST(XsonJsonTest,ParseFile1)
{
    auto fs = std::ifstream{"./test/xson/test1.json"};
    auto ob = json::parse(fs);
    TRACE("test1.json: "s << ob);

    ASSERT_EQ(type::array, ob["required"s].type());
    ASSERT_EQ(type::string, ob["required"s][2].type());
    const xson::string_type price = ob["required"s][2];
    EXPECT_EQ("price"s, price);

    ASSERT_EQ(type::object, ob["properties"s].type());
    ASSERT_EQ(type::object, ob["properties"s]["price"s].type());
    ASSERT_EQ(type::int64, ob["properties"s]["price"s]["minimum"s].type());
    const xson::integer_type minimum = ob["properties"s]["price"s]["minimum"s];
    EXPECT_EQ(0, minimum);
}

TEST(XsonJsonTest,ParseFile2)
{
    auto fs = std::ifstream{"./test/xson/test2.json"};
    auto ob = json::parse(fs);
    TRACE("test2.json: "s << ob);

    const xson::boolean_type alive = ob["isAlive"s];
    EXPECT_EQ(true, alive);
    ASSERT_EQ(type::boolean, ob["isAlive"s].type());

    const xson::null_type spouce = ob["spouse"s];
    EXPECT_EQ(nullptr, spouce);
    ASSERT_EQ(type::null, ob["spouse"s].type());
}
