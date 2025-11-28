// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::chrono;
using namespace std::string_literals;
using namespace xson;
using namespace xson::json;

namespace xson::object_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("Simple") = [] {
        auto ob = object
        {
            { "First",  "Tulppu"s },
            { "Second", "Elppu"s  },
            { "Third",  "Jalppu"s }
        };
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("First"s));
        require_true(ob.has("Second"s));
        require_true(ob.has("Third"s));
        require_true(ob["First"s].is_string());
        require_true(ob["Second"s].is_string());
        require_true(ob["Third"s].is_string());
    };

    test_case("Mix") = [] {
        auto o1 = object{"Integer"s, 123456789};
        xson::trace::trace(json::stringify(o1));

        check_false(o1.empty());
        check_true(o1.has("Integer"s));
        check_true(o1["Integer"s].is_integer());
        const int i1 = o1["Integer"s];
        check_eq(123456789, i1);

        auto o2 = object
        {
            {"Integer"s, 987654321},
            {"Double"s,  21.12},
            {"Boolean"s, false},
            {"Object1"s, o1},
            {"Object2"s, {{"Boolean"s, false}, {"String"s, "4"s}}},
            {"Array"s, {1, 2, 3, 4, 5, 6}}
        };
        xson::trace::trace(json::stringify(o2));

        require_false(o2.empty());
        require_true(o2.has("Integer"s));
        require_true(o2.has("Double"s));
        require_true(o2.has("Boolean"s));
        require_true(o2.has("Object1"s));
        require_true(o2.has("Object2"s));
        require_true(o2.has("Array"s));

        require_true(o2["Double"s].is_number());
        const double d = o2["Double"s];
        require_eq(21.12, d);

        require_true(o2["Object1"s].is_object());
        require_true(o2["Object1"s]["Integer"s].is_integer());
        const int i2 = o2["Object1"s]["Integer"s];
        require_eq(123456789, i2);

        require_true(o2["Object2"s].is_object());

        require_true(o2["Object2"s]["Boolean"s].is_boolean());
        bool b = o2["Object2"s]["Boolean"s];
        require_eq(false, b);

        require_true(o2["Object2"s]["String"s].is_string());
        std::string s = o2["Object2"s]["String"s];
        require_eq("4"s, s);
    };

    test_case("Array") = [] {
        auto arr = std::array<int,9>{1,2,3,4,5,6,7,8,9};
        auto ob = object{"Array"s, arr};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("Array"s));
        require_true(ob["Array"s].is_array());
        int idx{0};
        for(const auto& a : arr)
        {
            require_true(ob["Array"s][idx].is_integer());
            const int i = ob["Array"s][idx];
            require_eq(a, i);
            ++idx;
        }
    };

    test_case("Vector") = [] {
        auto vec = std::vector<std::string>{"a","b","c","d","e","f","g","h","i"};
        auto ob = object{"Vector"s, vec};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("Vector"s));
        require_true(ob["Vector"s].is_array());
        int idx{0};
        for(const auto& v : vec)
        {
            require_true(ob["Vector"s][idx].is_string());
            const std::string s = ob["Vector"s][idx];
            require_eq(v, s);
            ++idx;
        }
    };

    test_case("CArray") = [] {
        double arr[] = {1.0, 1.1, 1.12, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
        auto ob = object{"CArray"s, arr};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("CArray"s));
        require_true(ob["CArray"s].is_array());
        int idx{0};
        for(const auto& a : arr)
        {
            require_true(ob["CArray"s][idx].is_number());
            const double d = ob["CArray"s][idx];
            require_eq(a, d);
            ++idx;
        }
    };

    test_case("BooleanTrue") = [] {
        auto ob = object{"True"s, true};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("True"s));
        require_true(ob["True"s].is_boolean());
        const bool b = ob["True"s];
        require_eq(true, b);
    };

    test_case("BooleanFalse") = [] {
        auto ob = object{"False"s, false};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("False"s));
        require_true(ob["False"s].is_boolean());
        const bool b = ob["False"s];
        require_eq(false, b);
    };

    test_case("Null") = [] {
        auto ob = object{"Null"s, nullptr};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("Null"s));
        require_true(ob["Null"s].is_null());
    };

    test_case("Date") = [] {
        auto now = system_clock::now();
        auto ob = object{"Date"s, now};
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("Date"s));
        require_true(ob["Date"s].is_timestamp());
        const system_clock::time_point tp = ob["Date"s];
        require_eq(duration_cast<milliseconds>(now.time_since_epoch()), duration_cast<milliseconds>(tp.time_since_epoch()));
        require_eq(duration_cast<milliseconds>(now.time_since_epoch()).count(), duration_cast<milliseconds>(tp.time_since_epoch()).count());
    };

    test_case("ObjectWithArray") = [] {
        auto ob = object
        {
            "ObjectArray"s, std::array<object,3>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
        };
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("ObjectArray"s));
        require_true(ob["ObjectArray"s].is_array());
        require_true(ob["ObjectArray"s][0].is_object());
        require_true(ob["ObjectArray"s][0]["A"s].is_integer());
        require_true(ob["ObjectArray"s][1].is_object());
        require_true(ob["ObjectArray"s][1]["B"s].is_integer());
        require_true(ob["ObjectArray"s][2].is_object());
        require_true(ob["ObjectArray"s][2]["C"s].is_integer());
    };

    test_case("ObjectWithVector1") = [] {
        auto ob = object
        {
            "Kids",
            std::vector<object>
            {
                { "Name", "Tulppu"s },
                { "Name", "Elppu"s  },
                { "Name", "Jalppu"s }
            }
        };
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("Kids"s));
        require_true(ob["Kids"s].is_array());
        require_true(ob["Kids"s][0].is_object());
        require_true(ob["Kids"s][1].is_object());
        require_true(ob["Kids"s][2].is_object());
        require_true(ob["Kids"s][0]["Name"s].is_string());
        require_true(ob["Kids"s][1]["Name"s].is_string());
        require_true(ob["Kids"s][2]["Name"s].is_string());
    };

    test_case("ObjectWithVector2") = [] {
        auto ob = object
        {
            "ObjectVector"s, std::vector<object>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
        };
        xson::trace::trace(json::stringify(ob));
        require_false(ob.empty());
        require_true(ob.has("ObjectVector"s));
        require_true(ob["ObjectVector"s].is_array());
        require_true(ob["ObjectVector"s][0].is_object());
        require_true(ob["ObjectVector"s][0]["A"s].is_integer());
        require_true(ob["ObjectVector"s][1].is_object());
        require_true(ob["ObjectVector"s][1]["B"s].is_integer());
        require_true(ob["ObjectVector"s][2].is_object());
        require_true(ob["ObjectVector"s][2]["C"s].is_integer());
    };

    test_case("Complex") = [] {
        auto ob = object
        {
            { "Ruoka",  true                         },
            { "Onni",   false                        },
            { "Tulppu", 1                            },
            { "Elppu",  2                            },
            { "Jalppu", 3                            },
            { "Ages",   std::vector<int>{39, 40, 9, 5, 2} }
        };
        xson::trace::trace(json::stringify(ob));

        require_true(ob.has("Ruoka"s));
        require_true(ob["Ruoka"s].is_boolean());
        const bool b = ob["Ruoka"s];
        require_eq(true, b);

        require_true(ob.has("Tulppu"s));
        require_true(ob["Tulppu"s].is_integer());
        const int i1 = ob["Tulppu"s];
        require_eq(1, i1);

        require_true(ob.has("Ages"s));
        require_true(ob["Ages"s].is_array());

        require_true(ob["Ages"s][4].is_integer());
        const int i2 = ob["Ages"s][4];
        require_eq(2, i2);
    };

    test_case("Plus") = [] {
        auto o1 = object
        {
            { "A"s, 1},
            { "B"s, 2},
        };
        require_eq(static_cast<int>(o1["A"s]), 1);
        require_eq(static_cast<int>(o1["B"s]), 2);

        auto o2 = object
        {
            { "C"s, 3},
            { "D"s, 4},
        };
        require_eq(static_cast<int>(o2["C"s]), 3);
        require_eq(static_cast<int>(o2["D"s]), 4);

        o1 += std::move(o2);
        auto ob3 = std::move(o1);
        require_eq(static_cast<int>(ob3["A"s]), 1);
        require_eq(static_cast<int>(ob3["B"s]), 2);
        require_eq(static_cast<int>(ob3["C"s]), 3);
        require_eq(static_cast<int>(ob3["D"s]), 4);

        auto ob4 = ob3 + object{"array", std::vector<int>{39, 40, 9, 5, 2}};
        require_eq(static_cast<int>(ob4["A"s]), 1);
        require_eq(static_cast<int>(ob4["B"s]), 2);
        require_eq(static_cast<int>(ob4["C"s]), 3);
        require_eq(static_cast<int>(ob4["D"s]), 4);
        require_eq(static_cast<int>(ob4["array"s][0]), 39);
        require_eq(static_cast<int>(ob4["array"s][1]), 40);
        require_eq(static_cast<int>(ob4["array"s][2]),  9);
        require_eq(static_cast<int>(ob4["array"s][3]),  5);
        require_eq(static_cast<int>(ob4["array"s][4]),  2);
    };

    test_case("InitializerList1") = [] {
        auto ob = object
        {
            "List", {1, 2, 3, 4, 5}
        };
        xson::trace::trace(json::stringify(ob));
        require_true(ob["List"s].is_array());
        require_true(ob["List"s][0].is_integer());
        require_true(ob["List"s][1].is_integer());
        require_true(ob["List"s][2].is_integer());
        require_true(ob["List"s][3].is_integer());
        require_true(ob["List"s][4].is_integer());
    };

    test_case("InitializerList2") = [] {
        auto ob = object
        {
            "List", {{"A"s,1},{"B"s,false},{"C"s,3.0},{"D"s,"4"s}}
        };
        xson::trace::trace(json::stringify(ob));
        require_true(ob["List"s].is_object());
        require_true(ob["List"s]["A"s].is_integer());
        require_true(ob["List"s]["B"s].is_boolean());
        require_true(ob["List"s]["C"s].is_number());
    };

    test_case("InitializerList3") = [] {
        auto ob = object
        {
            {"id",  987654321},
            {"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}},
            {"array",  {1, 2, 3, 4, 5}},
            {"bool", true},
            {"name", "Hepokatti Maantiella"s}
        };
        std::clog << ob << std::endl;
    };

    test_case("Match1") = [] {
        auto ob = object
        {
            {"id",  987654321},
            {"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}},
            {"array",  {1, 2, 3, 4, 5}},
            {"bool", true},
            {"name", "Hepokatti Maantiella"s}
        };
        xson::trace::trace(json::stringify(ob));
        require_true(ob.match(object{"id", 987654321}));
        require_false(ob.match(object{"XXXX", 1}));
        require_false(ob.match(object{"id", 1}));
        require_true(ob.match(object{}));
        require_false(ob.match(object{"bool", false}));
        require_true(ob.match(object{"bool", true}));
        require_false(ob.match(object{"keitto", "XYZ"s}));
        require_true(ob.match(object{"array", {1, 2, 3, 4, 5}}));
        require_false(ob.match(object{"array", {1, 2, 3, 4, 6}}));
        require_false(ob.match(object{"name", "asd"s}));
        require_true(ob.match(object{"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}}));
        require_false(ob.match(object{"object", {{"A"s,1},{"B"s,13},{"C"s,3},{"D"s,4}}}));
        require_true(ob.match(object{"name", object{}}));
    };

    test_case("Match2") = [] {
        auto ob = object
        {
            {"id",  987654321},
        };
        require_true(ob.match(object{"id", 987654321}));
        require_false(ob.match(object{{"id", 987654321}, {"fail", 1}}));
    };

    test_case("Match3") = [] {
        auto o1 = object{{"id"s, 1}, {"A"s, 1}},
             o2 = object{{"id"s, 1}, {"A"s, 1}, {"B"s, 2}},
             o3 = object{{"id"s, 1}, {"A"s, 1}, {"B"s, 2}, {"C"s, 3}},
             s = object{"id"s, 1};

        require_true(o1.match(s));
        require_true(o2.match(s));
        require_true(o3.match(s));
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::object_test

