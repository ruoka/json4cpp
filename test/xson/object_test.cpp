#include <iostream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace chrono;
using namespace string_literals;
using namespace xson;
using namespace json;

TEST(XsonObjectTest,Simple)
{
    auto ob = object
    {
        { "First",  "Tulppu"s },
        { "Second", "Elppu"s  },
        { "Third",  "Jalppu"s }
    };
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("First"s));
    ASSERT_TRUE(ob.has("Second"s));
    ASSERT_TRUE(ob.has("Third"s));
    ASSERT_TRUE(ob["First"s].is_string());
    ASSERT_TRUE(ob["Second"s].is_string());
    ASSERT_TRUE(ob["Third"s].is_string());
}

TEST(XsonObjectTest,Mix)
{
    auto o1 = object{"Integer"s, 123456789};
    TRACE(o1);

    EXPECT_FALSE(o1.empty());
    EXPECT_TRUE(o1.has("Integer"s));
    EXPECT_TRUE(o1["Integer"s].is_integer());
    const int i1 = o1["Integer"s];
    EXPECT_EQ(123456789, i1);

    auto o2 = object
    {
        {"Integer"s, 987654321},
        {"Double"s,  21.12},
        {"Boolean"s, false},
        {"Object1"s, o1},
        {"Object2"s, {{"Boolean"s, false}, {"String"s, "4"s}}},
        {"Array"s, {1, 2, 3, 4, 5, 6}}
    };
    TRACE(o2);

    ASSERT_FALSE(o2.empty());
    ASSERT_TRUE(o2.has("Integer"s));
    ASSERT_TRUE(o2.has("Double"s));
    ASSERT_TRUE(o2.has("Boolean"s));
    ASSERT_TRUE(o2.has("Object1"s));
    ASSERT_TRUE(o2.has("Object2"s));
    ASSERT_TRUE(o2.has("Array"s));

    ASSERT_TRUE(o2["Double"s].is_number());
    const double d = o2["Double"s];
    ASSERT_EQ(21.12, d);

    ASSERT_TRUE(o2["Object1"s].is_object());
    ASSERT_TRUE(o2["Object1"s]["Integer"s].is_integer());
    const int i2 = o2["Object1"s]["Integer"s];
    ASSERT_EQ(123456789, i2);

    ASSERT_TRUE(o2["Object2"s].is_object());

    ASSERT_TRUE(o2["Object2"s]["Boolean"s].is_boolean());
    bool b = o2["Object2"s]["Boolean"s];
    ASSERT_EQ(false, b);

    ASSERT_TRUE(o2["Object2"s]["String"s].is_string());
    string s = o2["Object2"s]["String"s];
    ASSERT_EQ("4"s, s);
}

TEST(XsonObjectTest,Array)
{
    auto arr = std::array<int,9>{1,2,3,4,5,6,7,8,9};
    auto ob =object{"Array"s, arr};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("Array"s));
    ASSERT_TRUE(ob["Array"s].is_array());
    int idx{0};
    for(const auto& a : arr)
    {
        ASSERT_TRUE(ob["Array"s][idx].is_integer());
        const int i = ob["Array"s][idx];
        ASSERT_EQ(a, i);
        ++idx;
    }
}

TEST(XsonObjectTest,Vector)
{
    auto vec = std::vector<string>{"a","b","c","d","e","f","g","h","i"};
    auto ob = object{"Vector"s, vec};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("Vector"s));
    ASSERT_TRUE(ob["Vector"s].is_array());
    int idx{0};
    for(const auto& v : vec)
    {
        ASSERT_TRUE(ob["Vector"s][idx].is_string());
        const string s = ob["Vector"s][idx];
        ASSERT_EQ(v, s);
        ++idx;
    }
}

TEST(XsonObjectTest,CArray)
{
    double arr[] = {1.0, 1.1, 1.12, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
    auto ob = object{"CArray"s, arr};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("CArray"s));
    ASSERT_TRUE(ob["CArray"s].is_array());
    int idx{0};
    for(const auto& a : arr)
    {
        ASSERT_TRUE(ob["CArray"s][idx].is_number());
        const double d = ob["CArray"s][idx];
        ASSERT_EQ(a, d);
        ++idx;
    }
}

TEST(XsonObjectTest,BooleanTrue)
{
    auto ob = object{"True"s, true};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("True"s));
    ASSERT_TRUE(ob["True"s].is_boolean());
    const bool b = ob["True"s];
    ASSERT_EQ(true, b);
}

TEST(XsonObjectTest,BooleanFalse)
{
    auto ob = object{"False"s, false};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("False"s));
    ASSERT_TRUE(ob["False"s].is_boolean());
    const bool b = ob["False"s];
    ASSERT_EQ(false, b);
}

TEST(XsonObjectTest,Null)
{
    auto ob = object{"Null"s, nullptr};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("Null"s));
    ASSERT_TRUE(ob["Null"s].is_null());
//  xson::null_type n = ob["Null"s];
//  ASSERT_EQ(nullptr, n);
}

TEST(XsonObjectTest,Date)
{
    auto now = system_clock::now();
    auto ob = object{"Date"s, now};
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("Date"s));
    ASSERT_TRUE(ob["Date"s].is_timestamp());
    const system_clock::time_point tp = ob["Date"s];
    ASSERT_EQ(duration_cast<milliseconds>(now.time_since_epoch()), duration_cast<milliseconds>(tp.time_since_epoch()));
    ASSERT_EQ(duration_cast<milliseconds>(now.time_since_epoch()).count(), duration_cast<milliseconds>(tp.time_since_epoch()).count());
}

TEST(XsonObjectTest,ObjectWithArray)
{
    auto ob = object
    {
        "ObjectArray"s, std::array<object,3>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
    };
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("ObjectArray"s));
    ASSERT_TRUE(ob["ObjectArray"s].is_array());
    ASSERT_TRUE(ob["ObjectArray"s][0].is_object());
    ASSERT_TRUE(ob["ObjectArray"s][0]["A"s].is_integer());
    ASSERT_TRUE(ob["ObjectArray"s][1].is_object());
    ASSERT_TRUE(ob["ObjectArray"s][1]["B"s].is_integer());
    ASSERT_TRUE(ob["ObjectArray"s][2].is_object());
    ASSERT_TRUE(ob["ObjectArray"s][2]["C"s].is_integer());
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
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("Kids"s));
    ASSERT_TRUE(ob["Kids"s].is_array());
    ASSERT_TRUE(ob["Kids"s][0].is_object());
    ASSERT_TRUE(ob["Kids"s][1].is_object());
    ASSERT_TRUE(ob["Kids"s][2].is_object());
    ASSERT_TRUE(ob["Kids"s][0]["Name"s].is_string());
    ASSERT_TRUE(ob["Kids"s][1]["Name"s].is_string());
    ASSERT_TRUE(ob["Kids"s][2]["Name"s].is_string());
}

TEST(XsonObjectTest,ObjectWithVector2)
{
    auto ob = object
    {
        "ObjectVector"s, vector<object>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
    };
    TRACE(ob);
    ASSERT_FALSE(ob.empty());
    ASSERT_TRUE(ob.has("ObjectVector"s));
    ASSERT_TRUE(ob["ObjectVector"s].is_array());
    ASSERT_TRUE(ob["ObjectVector"s][0].is_object());
    ASSERT_TRUE(ob["ObjectVector"s][0]["A"s].is_integer());
    ASSERT_TRUE(ob["ObjectVector"s][1].is_object());
    ASSERT_TRUE(ob["ObjectVector"s][1]["B"s].is_integer());
    ASSERT_TRUE(ob["ObjectVector"s][2].is_object());
    ASSERT_TRUE(ob["ObjectVector"s][2]["C"s].is_integer());
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

    ASSERT_TRUE(ob.has("Ruoka"s));
    ASSERT_TRUE(ob["Ruoka"s].is_boolean());
    const bool b = ob["Ruoka"s];
    ASSERT_EQ(true, b);

    ASSERT_TRUE(ob.has("Tulppu"s));
    ASSERT_TRUE(ob["Tulppu"s].is_integer());
    const int i1 = ob["Tulppu"s];
    ASSERT_EQ(1, i1);

    ASSERT_TRUE(ob.has("Ages"s));
    ASSERT_TRUE(ob["Ages"s].is_array());

    ASSERT_TRUE(ob["Ages"s][4].is_integer());
    const int i2 = ob["Ages"s][4];
    ASSERT_EQ(2, i2);
}

TEST(XsonObjectTest,Plus)
{
    auto o1 = object
    {
        { "A"s, 1},
        { "B"s, 2},
    };
    ASSERT_EQ(static_cast<int>(o1["A"s]), 1);
    ASSERT_EQ(static_cast<int>(o1["B"s]), 2);

    auto o2 = object
    {
        { "C"s, 3},
        { "D"s, 4},
    };
    ASSERT_EQ(static_cast<int>(o2["C"s]), 3);
    ASSERT_EQ(static_cast<int>(o2["D"s]), 4);

    o1 += std::move(o2);
    auto ob3 = std::move(o1);
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
    ASSERT_TRUE(ob["List"s].is_array());
    ASSERT_TRUE(ob["List"s][0].is_integer());
    ASSERT_TRUE(ob["List"s][1].is_integer());
    ASSERT_TRUE(ob["List"s][2].is_integer());
    ASSERT_TRUE(ob["List"s][3].is_integer());
    ASSERT_TRUE(ob["List"s][4].is_integer());
}

TEST(XsonObjectTest,InitializerList2)
{
    auto ob = object
    {
        "List", {{"A"s,1},{"B"s,false},{"C"s,3.0},{"D"s,"4"s}}
    };
    TRACE(ob);
    ASSERT_TRUE(ob["List"s].is_object());
    ASSERT_TRUE(ob["List"s]["A"s].is_integer());
    ASSERT_TRUE(ob["List"s]["B"s].is_boolean());
    ASSERT_TRUE(ob["List"s]["C"s].is_number());
}

TEST(XsonObjectTest,InitializerList3)
{
    auto ob = object
    {
        {"id",  987654321},
        {"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}},
        {"array",  {1, 2, 3, 4, 5}},
        {"bool", true},
        {"name", "Hepokatti Maantiella"s}
    };
    clog << ob << endl;
}

TEST(XsonObjectTest,Match1)
{
    auto ob = object
    {
        {"id",  987654321},
        {"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}},
        {"array",  {1, 2, 3, 4, 5}},
        {"bool", true},
        {"name", "Hepokatti Maantiella"s}
    };
    TRACE(ob);
    ASSERT_TRUE(ob.match(object{"id", 987654321}));
    ASSERT_FALSE(ob.match(object{"XXXX", 1}));
    ASSERT_FALSE(ob.match(object{"id", 1}));
    ASSERT_TRUE(ob.match(object{}));
    ASSERT_FALSE(ob.match(object{"bool", false}));
    ASSERT_TRUE(ob.match(object{"bool", true}));
    ASSERT_FALSE(ob.match(object{"keitto", "XYZ"s}));
    ASSERT_TRUE(ob.match(object{"array", {1, 2, 3, 4, 5}}));
    ASSERT_FALSE(ob.match(object{"array", {1, 2, 3, 4, 6}}));
    ASSERT_FALSE(ob.match(object{"name", "asd"s}));
    ASSERT_TRUE(ob.match(object{"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}}));
    ASSERT_FALSE(ob.match(object{"object", {{"A"s,1},{"B"s,13},{"C"s,3},{"D"s,4}}}));
    ASSERT_TRUE(ob.match(object{"name", object{}}));
}

TEST(XsonObjectTest,Match2)
{
    auto ob = object
    {
        {"id",  987654321},
    };
    ASSERT_TRUE(ob.match(object{"id", 987654321}));
    ASSERT_FALSE(ob.match(object{{"id", 987654321}, {"fail", 1}}));
}

TEST(XsonObjectTest,Match3)
{
    auto o1 = object{{"id"s, 1}, {"A"s, 1}},
         o2 = object{{"id"s, 1}, {"A"s, 1}, {"B"s, 2}},
         o3 = object{{"id"s, 1}, {"A"s, 1}, {"B"s, 2}, {"C"s, 3}},
         s = object{"id"s, 1};

    ASSERT_TRUE(o1.match(s));
    ASSERT_TRUE(o2.match(s));
    ASSERT_TRUE(o3.match(s));
}
