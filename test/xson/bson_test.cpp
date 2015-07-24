#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "xson/bson.hpp"
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace chrono;
using namespace xson;
using namespace bson;

TEST(XsonBsonTest,String)
{
    stringstream ss;
    ss << bson::object{"String", "1234567890"s};
    TRACE(ss.str());

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::string, ob["String"s].type());
    string s = ob["String"s];
    ASSERT_EQ("1234567890"s, s);
}

TEST(XsonBsonTest,Double)
{
    stringstream ss;
    ss << bson::object{"Double", 21.12};
    TRACE(ss.str());

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::number, ob["Double"s].type());
    double d = ob["Double"s];
    ASSERT_EQ(21.12, d);
}

TEST(XsonBsonTest,Boolean)
{
    stringstream ss;
    ss << bson::object{{"True", true},{"False", false}};
    TRACE(ss.str());

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::boolean, ob["True"s].type());
    bool t = ob["True"s];
    ASSERT_EQ(true, t);

    ASSERT_EQ(type::boolean, ob["False"s].type());
    bool f = ob["False"s];
    ASSERT_EQ(false, f);
}

TEST(XsonBsonTest,Date2String)
{
    auto now = system_clock::now();
    stringstream ss;
    ss << bson::object{"Date", now};
    TRACE(ss.str());

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::date, ob["Date"s].type());
    string d = ob["Date"s];
    ASSERT_EQ(to_string(now), d);
}

TEST(XsonBsonTest,Null)
{
    stringstream ss;
    ss << bson::object{"Null", nullptr};
    TRACE(ss.str());

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::null, ob["Null"s].type());
    string n = ob["Null"s];
    ASSERT_EQ("null"s, n);
}

TEST(XsonBsonTest,Int32)
{
    stringstream ss;
    ss << bson::object{
        {"Zero", int32_t{0}},
        {"Min", numeric_limits<int>::min()},
        {"Max", numeric_limits<int>::max()}
    };
    TRACE(ss.str());

    auto ob = bson::parse(ss);
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

TEST(XsonBsonTest,Int64)
{
    stringstream ss;
    ss << bson::object{
        {"Zero", int64_t{0}},
        {"Min", numeric_limits<long long>::min()},
        {"Max", numeric_limits<long long>::max()}
    };
    TRACE(ss.str());

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::int64, ob["Zero"s].type());
    long long zero = ob["Zero"s];
    ASSERT_EQ(0, zero);

    ASSERT_EQ(type::int64, ob["Min"s].type());
    long long min = ob["Min"s];
    ASSERT_EQ(numeric_limits<long long>::min(), min);

    ASSERT_EQ(type::int64, ob["Max"s].type());
    long long max = ob["Max"s];
    ASSERT_EQ(numeric_limits<long long>::max(), max);
}

TEST(XsonBsonTest,Array)
{
    auto ob1 = bson::object
    {
        {"Array"s, array<int,3>{1, 2, 3}}
    };
    TRACE("ob1:  " << ob1);
    auto str1 = bson::stringify(ob1);
    TRACE("str1: " << str1);

    auto ss = stringstream{str1};

    auto ob2 = bson::parse(ss);
    TRACE("ob2:  " << ob2);
    auto str2 = bson::stringify(ob2);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonBsonTest,Vector)
{
    auto ob1 = bson::object
    {
        {"Vector"s, vector<string>{"a", "b", "c", "d", "e", "f"}}
    };
    TRACE("ob1:  " << ob1);
    auto str1 = bson::stringify(ob1);
    TRACE("str1: " << str1);

    auto ss = stringstream{str1};

    auto ob2 = bson::parse(ss);
    TRACE("ob2:  " << ob2);
    auto str2 = bson::stringify(ob2);
    TRACE("str2: " << str2);

    EXPECT_EQ(str1, str2);
}

TEST(XsonBsonTest,Complex)
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

    auto ob = bson::parse(ss);
    TRACE(ob);

    ASSERT_EQ(type::boolean, ob["Ruoka"s].type());
    const bool b = ob["Ruoka"s];
    ASSERT_EQ(true, b);

    ASSERT_EQ(type::int32, ob["Tulppu"s].type());
    const int i1 = ob["Tulppu"s];
    ASSERT_EQ(1, i1);

    ASSERT_EQ(type::array, ob["Ages"s].type());

    ASSERT_EQ(type::int32, ob["Ages"s][4].type());
    const int i2 = ob["Ages"s][4];
    ASSERT_EQ(2, i2);
}

TEST(XsonJsonTest,ParseFile)
{
    using namespace bson;
    ifstream fs{"./test/xson/test3.bson"};
    while(fs && !fs.eof())
    {
        auto ob = bson::parse(fs);
        clog << json::stringify(ob) << endl;
        fs >> ws;
    }
}
