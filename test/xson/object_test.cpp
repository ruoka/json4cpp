#include <iostream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace chrono;
using namespace string_literals;
using namespace xson;
using namespace json;

TEST(XsonObjectTest,Object)
{
    auto ob = object
    {
        { "First",  "Tulppu"s },
        { "Second", "Elppu"s  },
        { "Third",  "Jalppu"s }
    };
    TRACE(ob);
    ASSERT_EQ(type::string, ob["First"s].type());
    ASSERT_EQ(type::string, ob["Second"s].type());
    ASSERT_EQ(type::string, ob["Third"s].type());
}

TEST(XsonObjectTest,Mix)
{
    auto ob1 = object{"Integer"s, 123456789};
    TRACE(ob1);

    ASSERT_EQ(type::int32, ob1["Integer"s].type());
    const int i1 = ob1["Integer"s];
    ASSERT_EQ(123456789, i1);

    auto ob2 = object
    {
        {"Integer"s, 987654321},
        {"Double"s,  21.12},
        {"Boolean"s, false},
        {"Object1"s, ob1},
        {"Object2"s, object{{"Boolean"s, false}, {"String"s, "4"s}}},
    };
    TRACE(ob2);

    ASSERT_EQ(type::number, ob2["Double"s].type());
    const double d = ob2["Double"s];
    ASSERT_EQ(21.12, d);

    ASSERT_EQ(type::object, ob2["Object1"s].type());

    ASSERT_EQ(type::int32, ob2["Object1"s]["Integer"s].type());
    const int i2 = ob2["Object1"s]["Integer"s];
    ASSERT_EQ(123456789, i2);

    ASSERT_EQ(type::object, ob2["Object2"s].type());

    ASSERT_EQ(type::boolean, ob2["Object2"s]["Boolean"s].type());
    bool b = ob2["Object2"s]["Boolean"s];
    ASSERT_EQ(false, b);

    ASSERT_EQ(type::string, ob2["Object2"s]["String"s].type());
    string s = ob2["Object2"s]["String"s];
    ASSERT_EQ("4"s, s);
}

TEST(XsonObjectTest,Array)
{
    array<int,9> arr = {1,2,3,4,5,6,7,8,9};
    object ob{"Array"s, arr};
    TRACE(ob);
    ASSERT_EQ(type::array, ob["Array"s].type());
    int idx{0};
    for(const auto& a : arr)
    {
        ASSERT_EQ(type::int32, ob["Array"s][idx].type());
        const int i = ob["Array"s][idx];
        ASSERT_EQ(a, i);
        ++idx;
    }
}

TEST(XsonObjectTest,Vector)
{
    vector<string> vec = {"a","b","c","d","e","f","g","h","i"};
    object ob{"Vector"s, vec};
    TRACE(ob);
    ASSERT_EQ(type::array, ob["Vector"s].type());
    int idx{0};
    for(const auto& v : vec)
    {
        ASSERT_EQ(type::string, ob["Vector"s][idx].type());
        const string s = ob["Vector"s][idx];
        ASSERT_EQ(v, s);
        ++idx;
    }
}

TEST(XsonObjectTest,CArray)
{
    double arr[] = {1.0, 1.1, 1.12, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
    object ob{"CArray"s, arr};
    TRACE(ob);
    ASSERT_EQ(type::array, ob["CArray"s].type());
    int idx{0};
    for(const auto& a : arr)
    {
        ASSERT_EQ(type::number, ob["CArray"s][idx].type());
        const double d = ob["CArray"s][idx];
        ASSERT_EQ(a, d);
        ++idx;
    }
}

TEST(XsonObjectTest,BooleanTrue)
{
    auto ob = object{"True"s, true};
    TRACE(ob);
    ASSERT_EQ(type::boolean, ob["True"s].type());
    const bool b = ob["True"s];
    ASSERT_EQ(true, b);
    const string s = ob["True"s];
    ASSERT_EQ("true"s, s);
}

TEST(XsonObjectTest,BooleanFalse)
{
    auto ob = object{"False"s, false};
    TRACE(ob);
    ASSERT_EQ(type::boolean, ob["False"s].type());
    const bool b = ob["False"s];
    ASSERT_EQ(false, b);
    const string s = ob["False"s];
    ASSERT_EQ("false"s, s);
}

TEST(XsonObjectTest,Null)
{
    auto ob = object{"Null"s, nullptr};
    TRACE(ob);
    ASSERT_EQ(type::null, ob["Null"s].type());
    const string s = ob["Null"s];
    ASSERT_EQ("null"s, s);
}

TEST(XsonObjectTest,Date)
{
    auto now = system_clock::now();
    auto ob = object{"Date"s, now};

    TRACE(ob);
    ASSERT_EQ(type::date, ob["Date"s].type());

    const string s = ob["Date"s];
    ASSERT_EQ(to_string(now), s);

    const system_clock::time_point tp = ob["Date"s];
    ASSERT_EQ(duration_cast<milliseconds>(now.time_since_epoch()), duration_cast<milliseconds>(tp.time_since_epoch()));
    ASSERT_EQ(duration_cast<milliseconds>(now.time_since_epoch()).count(), duration_cast<milliseconds>(tp.time_since_epoch()).count());
}

TEST(XsonObjectTest,ObjectWithArray)
{
    auto ob = object
    {
        "ObjectArray"s, array<object,3>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
    };
    TRACE(ob);
    ASSERT_EQ(type::array, ob["ObjectArray"s].type());
    ASSERT_EQ(type::object, ob["ObjectArray"s][0].type());
    ASSERT_EQ(type::int32, ob["ObjectArray"s][0]["A"s].type());
    ASSERT_EQ(type::object, ob["ObjectArray"s][1].type());
    ASSERT_EQ(type::int32, ob["ObjectArray"s][1]["B"s].type());
    ASSERT_EQ(type::object, ob["ObjectArray"s][2].type());
    ASSERT_EQ(type::int32, ob["ObjectArray"s][2]["C"s].type());
}

TEST(XsonObjectTest,ObjectWithVector1)
{
    auto ob = object
    {
        "Kids",
        vector<object>
        {
            { "Name", "Tulppu"s },
            { "Name", "Elppu"s  },
            { "Name", "Jalppu"s }
        }
    };
    TRACE(ob);
    ASSERT_EQ(type::array,  ob["Kids"s].type());
    ASSERT_EQ(type::object, ob["Kids"s][0].type());
    ASSERT_EQ(type::object, ob["Kids"s][1].type());
    ASSERT_EQ(type::object, ob["Kids"s][2].type());
    ASSERT_EQ(type::string, ob["Kids"s][0]["Name"s].type());
    ASSERT_EQ(type::string, ob["Kids"s][1]["Name"s].type());
    ASSERT_EQ(type::string, ob["Kids"s][2]["Name"s].type());
}

TEST(XsonObjectTest,ObjectWithVector2)
{
    auto ob = object
    {
        "ObjectVector"s, vector<object>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
    };
    TRACE(ob);
    ASSERT_EQ(type::array, ob["ObjectVector"s].type());
    ASSERT_EQ(type::object, ob["ObjectVector"s][0].type());
    ASSERT_EQ(type::int32, ob["ObjectVector"s][0]["A"s].type());
    ASSERT_EQ(type::object, ob["ObjectVector"s][1].type());
    ASSERT_EQ(type::int32, ob["ObjectVector"s][1]["B"s].type());
    ASSERT_EQ(type::object, ob["ObjectVector"s][2].type());
    ASSERT_EQ(type::int32, ob["ObjectVector"s][2]["C"s].type());
}

TEST(XsonObjectTest,Complex)
{
    auto ob = object
    {
        { "Ruoka",  true                         },
        { "Onni",   false                        },
        { "Tulppu", 1                            },
        { "Elppu",  2                            },
        { "Jalppu", 3                            },
        { "Ages",   vector<int>{39, 40, 9, 5, 2} }
    };
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

TEST(XsonObjectTest,Plus)
{
    auto ob1 = object
    {
        { "A"s, 1},
        { "B"s, 2},
    };
    ASSERT_EQ(static_cast<int>(ob1["A"s]), 1);
    ASSERT_EQ(static_cast<int>(ob1["B"s]), 2);

    auto ob2 = object
    {
        { "C"s, 3},
        { "D"s, 4},
    };
    ASSERT_EQ(static_cast<int>(ob2["C"s]), 3);
    ASSERT_EQ(static_cast<int>(ob2["D"s]), 4);

    auto ob3 = ob1 + ob2;
    ASSERT_EQ(static_cast<int>(ob3["A"s]), 1);
    ASSERT_EQ(static_cast<int>(ob3["B"s]), 2);
    ASSERT_EQ(static_cast<int>(ob3["C"s]), 3);
    ASSERT_EQ(static_cast<int>(ob3["D"s]), 4);

    auto ob4 = ob3 + object{"array", vector<int>{39, 40, 9, 5, 2}};
    ASSERT_EQ(static_cast<int>(ob4["A"s]), 1);
    ASSERT_EQ(static_cast<int>(ob4["B"s]), 2);
    ASSERT_EQ(static_cast<int>(ob4["C"s]), 3);
    ASSERT_EQ(static_cast<int>(ob4["D"s]), 4);
    ASSERT_EQ(static_cast<int>(ob4["array"s][0]), 39);
    ASSERT_EQ(static_cast<int>(ob4["array"s][1]), 40);
    ASSERT_EQ(static_cast<int>(ob4["array"s][2]),  9);
    ASSERT_EQ(static_cast<int>(ob4["array"s][3]),  5);
    ASSERT_EQ(static_cast<int>(ob4["array"s][4]),  2);
}

TEST(XsonObjectTest,InitializerList1)
{
    auto ob = object
    {
        "List", {1, 2, 3, 4, 5}
    };
    TRACE(ob);
    clog << ob << endl;
    ASSERT_EQ(type::array, ob["List"s].type());
    ASSERT_EQ(type::int32, ob["List"s][0].type());
    ASSERT_EQ(type::int32, ob["List"s][1].type());
    ASSERT_EQ(type::int32, ob["List"s][2].type());
    ASSERT_EQ(type::int32, ob["List"s][3].type());
    ASSERT_EQ(type::int32, ob["List"s][4].type());
}

TEST(XsonObjectTest,InitializerList2)
{
    auto ob = object
    {
        "List", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}
    };
    clog << ob << endl;
    TRACE(ob);
    ASSERT_EQ(type::array, ob["List"s].type());
    ASSERT_EQ(type::object, ob["List"s][0].type());
    ASSERT_EQ(type::object, ob["List"s][1].type());
    ASSERT_EQ(type::object, ob["List"s][2].type());
    ASSERT_EQ(type::object, ob["List"s][3].type());
    ASSERT_EQ(type::int32, ob["List"s][0]["A"s].type());
    ASSERT_EQ(type::int32, ob["List"s][1]["B"s].type());
    ASSERT_EQ(type::int32, ob["List"s][2]["C"s].type());
    ASSERT_EQ(type::int32, ob["List"s][3]["D"s].type());
}

TEST(XsonObjectTest,InitializerList3)
{
    auto ob = object
    {
        {"_id",  987654321},
        {"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}},
        {"array",  {1, 2, 3, 4, 5}},
        {"bool", true},
        {"name", "Hepokatti Maantiella"s}
    };
    clog << ob << endl;
}
