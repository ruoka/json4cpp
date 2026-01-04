// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::chrono;
using namespace std::string_literals;
using namespace xson;
using namespace xson::json;

using xson::json::operator <<;
using xson::json::operator >>;

namespace xson::object_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;
    using xson::json::operator <<;
    using xson::json::operator >>;

    test_case("Simple, [xson]") = [] {
        auto ob = object
        {
            { "First",  "Tulppu"s },
            { "Second", "Elppu"s  },
            { "Third",  "Jalppu"s }
        };
        succeed(xson::json::stringify(ob, 2));
        require_false(ob.empty());
        require_true(ob.has("First"s));
        require_true(ob.has("Second"s));
        require_true(ob.has("Third"s));
        require_true(ob["First"s].is_string());
        require_true(ob["Second"s].is_string());
        require_true(ob["Third"s].is_string());
    };

    test_case("Mix, [xson]") = [] {
        auto o1 = object{"Integer"s, 123456789};
        succeed(xson::json::stringify(o1, 2));

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
        succeed(xson::json::stringify(o2, 2));

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


    test_case("Vector, [xson]") = [] {
        auto vec = std::vector<std::string>{"a","b","c","d","e","f","g","h","i"};
        auto ob = object{"Vector"s, vec};
        succeed(xson::json::stringify(ob, 2));
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

    test_case("CArray, [xson]") = [] {
        std::vector<double> arr = {1.0, 1.1, 1.12, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
        auto ob = object{"CArray"s, arr};
        succeed(xson::json::stringify(ob, 2));
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

    test_case("BooleanTrue, [xson]") = [] {
        auto ob = object{"True"s, true};
        succeed(xson::json::stringify(ob, 2));
        require_false(ob.empty());
        require_true(ob.has("True"s));
        require_true(ob["True"s].is_boolean());
        const bool b = ob["True"s];
        require_eq(true, b);
    };

    test_case("BooleanFalse, [xson]") = [] {
        auto ob = object{"False"s, false};
        succeed(xson::json::stringify(ob, 2));
        require_false(ob.empty());
        require_true(ob.has("False"s));
        require_true(ob["False"s].is_boolean());
        const bool b = ob["False"s];
        require_eq(false, b);
    };

    test_case("Null, [xson]") = [] {
        auto ob = object{"Null"s, nullptr};
        succeed(xson::json::stringify(ob, 2));
        require_false(ob.empty());
        require_true(ob.has("Null"s));
        require_true(ob["Null"s].is_null());
    };

    test_case("Date, [xson]") = [] {
        auto now = system_clock::now();
        auto ob = object{"Date"s, now};
        succeed(xson::json::stringify(ob, 2));
        require_false(ob.empty());
        require_true(ob.has("Date"s));
        require_true(ob["Date"s].is_timestamp());
        const system_clock::time_point tp = ob["Date"s];
        require_eq(duration_cast<milliseconds>(now.time_since_epoch()), duration_cast<milliseconds>(tp.time_since_epoch()));
        require_eq(duration_cast<milliseconds>(now.time_since_epoch()).count(), duration_cast<milliseconds>(tp.time_since_epoch()).count());
    };


    test_case("ObjectWithVector1, [xson]") = [] {
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
        succeed(xson::json::stringify(ob, 2));
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

    test_case("ObjectWithVector2, [xson]") = [] {
        auto ob = object
        {
            "ObjectVector"s, std::vector<object>{object{"A"s, 1}, object{"B"s, 2}, object{"C"s, 3}}
        };
        succeed(xson::json::stringify(ob, 2));
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

    test_case("Complex, [xson]") = [] {
        auto ob = object
        {
            { "Ruoka",  true                         },
            { "Onni",   false                        },
            { "Tulppu", 1                            },
            { "Elppu",  2                            },
            { "Jalppu", 3                            },
            { "Ages",   std::vector<int>{39, 40, 9, 5, 2} }
        };
        succeed(xson::json::stringify(ob, 2));

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

    test_case("Plus, [xson]") = [] {
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
        const auto& arr = ob4["array"s].get<array>();
        std::vector<int> arr_values;
        arr_values.reserve(arr.size());
        for (const auto& val : arr) {
            arr_values.push_back(static_cast<int>(val));
        }
        require_container_eq(arr_values, std::vector<int>{39, 40, 9, 5, 2});
    };

    test_case("InitializerList1, [xson]") = [] {
        auto ob = object
        {
            "List", {1, 2, 3, 4, 5}
        };
        succeed(xson::json::stringify(ob, 2));
        require_true(ob["List"s].is_array());
        require_true(ob["List"s][0].is_integer());
        require_true(ob["List"s][1].is_integer());
        require_true(ob["List"s][2].is_integer());
        require_true(ob["List"s][3].is_integer());
        require_true(ob["List"s][4].is_integer());
    };

    test_case("InitializerList2, [xson]") = [] {
        auto ob = object
        {
            "List", {{"A"s,1},{"B"s,false},{"C"s,3.0},{"D"s,"4"s}}
        };
        succeed(xson::json::stringify(ob, 2));
        require_true(ob["List"s].is_object());
        require_true(ob["List"s]["A"s].is_integer());
        require_true(ob["List"s]["B"s].is_boolean());
        require_true(ob["List"s]["C"s].is_number());
    };

    test_case("InitializerList3, [xson]") = [] {
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

    test_case("Match1, [xson]") = [] {
        auto ob = object
        {
            {"id",  987654321},
            {"object", {{"A"s,1},{"B"s,2},{"C"s,3},{"D"s,4}}},
            {"array",  {1, 2, 3, 4, 5}},
            {"bool", true},
            {"name", "Hepokatti Maantiella"s}
        };
        succeed(json::stringify(ob));
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

    test_case("Match2, [xson]") = [] {
        auto ob = object
        {
            {"id",  987654321},
        };
        require_true(ob.match(object{"id", 987654321}));
        require_false(ob.match(object{{"id", 987654321}, {"fail", 1}}));
    };

    test_case("Match3, [xson]") = [] {
        auto o1 = object{{"id"s, 1}, {"A"s, 1}},
             o2 = object{{"id"s, 1}, {"A"s, 1}, {"B"s, 2}},
             o3 = object{{"id"s, 1}, {"A"s, 1}, {"B"s, 2}, {"C"s, 3}},
             s = object{"id"s, 1};

        require_true(o1.match(s));
        require_true(o2.match(s));
        require_true(o3.match(s));
    };

    test_case("DefaultConstructor, [xson]") = [] {
        auto ob = object{};
        // Default constructor creates an empty map (first variant alternative)
        // Verify the actual behavior
        succeed("Default object is_object: "s + (ob.is_object() ? "true"s : "false"s));
        succeed("Default object has_objects: "s + (ob.has_objects() ? "true"s : "false"s));
        succeed("Default object has_value: "s + (ob.has_value() ? "true"s : "false"s));
        require_true(ob.empty());
        require_true(ob.is_object());  // Default variant holds first alternative (map)
        require_true(ob.has_objects());
        require_false(ob.has_value());
        require_false(ob.is_array());
        require_eq(0u, ob.size());
    };

    test_case("CopyConstructor, [xson]") = [] {
        auto o1 = object{{"A"s, 1}, {"B"s, 2}};
        auto o2 = o1;  // Copy constructor
        require_eq(o1, o2);
        require_true(o2.has("A"s));
        require_true(o2.has("B"s));
        check_eq(static_cast<int>(o2["A"s]), 1);
        check_eq(static_cast<int>(o2["B"s]), 2);
    };

    test_case("MoveConstructor, [xson]") = [] {
        auto o1 = object{{"A"s, 1}, {"B"s, 2}};
        auto o2 = std::move(o1);  // Move constructor
        require_true(o2.has("A"s));
        require_true(o2.has("B"s));
        check_eq(static_cast<int>(o2["A"s]), 1);
        check_eq(static_cast<int>(o2["B"s]), 2);
        require_true(o1.empty());  // o1 should be empty after move
    };

    test_case("CopyAssignment, [xson]") = [] {
        auto o1 = object{{"A"s, 1}, {"B"s, 2}};
        auto o2 = object{{"C"s, 3}};
        o2 = o1;  // Copy assignment
        require_eq(o1, o2);
        require_true(o2.has("A"s));
        require_true(o2.has("B"s));
        require_false(o2.has("C"s));
    };

    test_case("MoveAssignment, [xson]") = [] {
        auto o1 = object{{"A"s, 1}, {"B"s, 2}};
        auto o2 = object{{"C"s, 3}};
        o2 = std::move(o1);  // Move assignment
        require_true(o2.has("A"s));
        require_true(o2.has("B"s));
        require_false(o2.has("C"s));
    };

    test_case("AssignmentFromArray, [xson]") = [] {
        auto ob = object{{"A"s, 1}};
        auto arr = array{object{1}, object{2}, object{3}};
        ob = arr;
        require_true(ob.is_array());
        require_eq(3u, ob.size());
        require_eq(static_cast<int>(ob[0]), 1);
        require_eq(static_cast<int>(ob[1]), 2);
        require_eq(static_cast<int>(ob[2]), 3);
    };

    test_case("AssignmentFromValue, [xson]") = [] {
        auto ob = object{{"A"s, 1}};
        ob = 42;
        require_true(ob.is_integer());
        require_eq(42, static_cast<int>(ob));
        
        ob = "test"s;
        require_true(ob.is_string());
        require_eq("test"s, static_cast<std::string>(ob));
        
        ob = true;
        require_true(ob.is_boolean());
        require_eq(true, static_cast<bool>(ob));
        
        ob = 3.14;
        require_true(ob.is_number());
        require_eq(3.14, static_cast<double>(ob));
        
        ob = nullptr;
        require_true(ob.is_null());
    };

    test_case("EmptyArray, [xson]") = [] {
        auto ob = object{"EmptyArray"s, std::vector<int>{}};
        require_true(ob["EmptyArray"s].is_array());
        require_true(ob["EmptyArray"s].empty());
        require_eq(0u, ob["EmptyArray"s].size());
    };

    test_case("EmptyObject, [xson]") = [] {
        auto ob = object{map{}};
        require_true(ob.is_object());
        require_true(ob.empty());
        require_eq(0u, ob.size());
    };

    test_case("Size, [xson]") = [] {
        auto ob1 = object{{"A"s, 1}, {"B"s, 2}, {"C"s, 3}};
        require_eq(3u, ob1.size());
        
        auto ob2 = object{"Array"s, {1, 2, 3, 4, 5}};
        require_eq(1u, ob2.size());
        require_eq(5u, ob2["Array"s].size());
        
        auto ob3 = object{42};
        require_eq(0u, ob3.size());  // Value has size 0
    };

    test_case("HasValue, [xson]") = [] {
        auto ob1 = object{42};
        require_true(ob1.has_value());
        require_false(ob1.has_objects());
        
        auto ob2 = object{{"A"s, 1}};
        require_false(ob2.has_value());
        require_true(ob2.has_objects());
        
        auto ob3 = object{array{object{1}, object{2}, object{3}}};
        require_false(ob3.has_value());
        require_false(ob3.has_objects());
    };

    test_case("HasObjects, [xson]") = [] {
        auto ob1 = object{{"A"s, 1}};
        require_true(ob1.has_objects());
        
        auto ob2 = object{42};
        require_false(ob2.has_objects());
        
        auto ob3 = object{array{object{1}, object{2}}};
        require_false(ob3.has_objects());
    };

    test_case("GetMethods, [xson]") = [] {
        // Test get<primitive>()
        auto ob1 = object{42};
        require_eq(42, std::get<std::int64_t>(ob1.get<primitive>()));
        
        // Test get<map>()
        auto ob2 = object{{"A"s, 1}, {"B"s, 2}};
        const auto& m = ob2.get<map>();
        require_eq(2u, m.size());
        require_true(m.contains("A"s));
        require_true(m.contains("B"s));
        
        // Test get<array>()
        auto ob3 = object{array{object{1}, object{2}, object{3}}};
        const auto& arr = ob3.get<array>();
        require_eq(3u, arr.size());
        
        // Test get<string_type>()
        auto ob4 = object{"test"s};
        require_eq("test"s, ob4.get<std::string>());
    };

    test_case("OperatorEquals, [xson]") = [] {
        auto o1 = object{{"A"s, 1}, {"B"s, 2}};
        auto o2 = object{{"A"s, 1}, {"B"s, 2}};
        auto o3 = object{{"A"s, 1}, {"B"s, 3}};
        
        require_true(o1 == o2);
        require_false(o1 == o3);
        
        auto a1 = object{array{object{1}, object{2}, object{3}}};
        auto a2 = object{array{object{1}, object{2}, object{3}}};
        auto a3 = object{array{object{1}, object{2}, object{4}}};
        
        require_true(a1 == a2);
        require_false(a1 == a3);
        
        auto v1 = object{42};
        auto v2 = object{42};
        auto v3 = object{43};
        
        require_true(v1 == v2);
        require_false(v1 == v3);
    };

    test_case("OperatorPlusWithArrays, [xson]") = [] {
        auto a1 = object{array{object{1}, object{2}}};
        auto a2 = object{array{object{3}, object{4}}};
        auto a3 = a1 + a2;
        
        require_true(a3.is_array());
        require_eq(3u, a3.size());  // 1, 2, [3, 4] (array appended as single element)
    };

    test_case("ArrayIndexOutOfBounds, [xson]") = [] {
        auto ob = object{array{object{1}, object{2}, object{3}}};
        
        // Const version
        require_nothrow([&]{ auto val = ob[2]; });
        require_throws_as([&]{ auto val = ob[3]; }, std::runtime_error{""});
        
        // Non-const version
        require_nothrow([&]{ ob[2] = 99; });
        require_throws_as([&]{ ob[3] = 99; }, std::runtime_error{""});
    };

    test_case("ArrayIndexOnNonArray, [xson]") = [] {
        auto ob = object{{"A"s, 1}};
        
        require_throws_as([&]{ auto val = ob[0]; }, std::runtime_error{""});
        require_throws_as([&]{ ob[0] = 99; }, std::runtime_error{""});
    };

    test_case("ObjectAccessOnNonObject, [xson]") = [] {
        auto ob = object{array{object{1}, object{2}, object{3}}};
        
        require_throws_as([&]{ auto val = ob["key"s]; }, std::runtime_error{""});
        require_throws_as([&]{ ob["key"s] = 99; }, std::runtime_error{""});
    };

    test_case("NonExistentKey, [xson]") = [] {
        auto ob = object{{"A"s, 1}};
        const auto& c_ob = ob;
        
        // Test that accessing non-existent key throws some exception using tester assertions
        require_throws([&]{ [[maybe_unused]] auto val = c_ob["Nonexistent"s]; });
        
        // Non-const creates new key if doesn't exist
        require_nothrow([&]{ ob["NewKey"s] = 42; });
        require_true(ob.has("NewKey"s));
    };

    test_case("ConversionOperators, [xson]") = [] {
        // Number
        auto ob1 = object{3.14};
        require_eq(3.14, static_cast<double>(ob1));
        
        // String
        auto ob2 = object{"hello"s};
        require_eq("hello"s, static_cast<std::string>(ob2));
        
        // Boolean
        auto ob3 = object{true};
        require_eq(true, static_cast<bool>(ob3));
        
        // Integer
        auto ob4 = object{42ll};
        require_eq(42ll, static_cast<std::int64_t>(ob4));
        
        // Timestamp
        auto now = std::chrono::system_clock::now();
        auto ob5 = object{now};
        require_eq(now, static_cast<std::chrono::system_clock::time_point>(ob5));
        
        // Value reference
        auto ob6 = object{42};
        const auto& val = static_cast<const primitive&>(ob6);
        require_true(std::holds_alternative<std::int64_t>(val));
    };

    test_case("MatchArray, [xson]") = [] {
        auto a1 = object{array{object{1}, object{2}, object{3}}};
        auto a2 = object{array{object{1}, object{2}, object{3}}};
        auto a3 = object{array{object{1}, object{2}, object{4}}};
        
        require_true(a1.match(a2));
        require_false(a1.match(a3));
        require_true(a1.match(object{array{}}));  // Empty subset matches
    };

    test_case("MatchWithOperators, [xson]") = [] {
        auto ob = object{42};
        
        // $eq
        require_true(ob.match(object{{"$eq"s, 42}}));
        require_false(ob.match(object{{"$eq"s, 43}}));
        
        // $ne
        require_true(ob.match(object{{"$ne"s, 43}}));
        require_false(ob.match(object{{"$ne"s, 42}}));
        
        // $lt
        require_true(ob.match(object{{"$lt"s, 50}}));
        require_false(ob.match(object{{"$lt"s, 40}}));
        
        // $lte
        require_true(ob.match(object{{"$lte"s, 42}}));
        require_true(ob.match(object{{"$lte"s, 50}}));
        require_false(ob.match(object{{"$lte"s, 40}}));
        
        // $gt
        require_true(ob.match(object{{"$gt"s, 40}}));
        require_false(ob.match(object{{"$gt"s, 50}}));
        
        // $gte
        require_true(ob.match(object{{"$gte"s, 42}}));
        require_true(ob.match(object{{"$gte"s, 40}}));
        require_false(ob.match(object{{"$gte"s, 50}}));
    };

    test_case("MatchValueWithObject, [xson]") = [] {
        auto ob = object{42};
        auto subset = object{{"$eq"s, 42}};
        require_true(ob.match(subset));
    };

    test_case("StringViewAccess, [xson]") = [] {
        auto ob = object{{"Key"s, 42}};
        std::string_view key{"Key"};
        
        require_true(ob.has(key));
        require_eq(42, static_cast<int>(ob[key]));
        
        // Non-const creates new if doesn't exist
        std::string_view new_key{"NewKey"};
        ob[new_key] = 99;
        require_true(ob.has("NewKey"s));
    };

    test_case("OperatorPlusMove, [xson]") = [] {
        auto o1 = object{{"A"s, 1}};
        auto o2 = object{{"B"s, 2}};
        auto o3 = o1 + std::move(o2);
        
        require_true(o3.has("A"s));
        require_true(o3.has("B"s));
        require_true(o2.empty());  // o2 moved from
    };

    test_case("OperatorPlusEqualsMove, [xson]") = [] {
        auto o1 = object{{"A"s, 1}};
        auto o2 = object{{"B"s, 2}};
        o1 += std::move(o2);
        
        require_true(o1.has("A"s));
        require_true(o1.has("B"s));
        require_false(o2.empty());  // Move does NOT guarantee that the map is left empty
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::object_test

