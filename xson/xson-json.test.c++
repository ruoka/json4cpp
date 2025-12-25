// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::chrono;
using namespace std::string_literals;
using namespace xson;

namespace xson::json_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("String") = [] {
        using xson::json::operator <<;
        auto obj = object{"String", "1234567890"s};
        auto ss = std::stringstream{};
        ss << obj;
        succeed(ss.str());

        auto ob = json::parse(ss.str());
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["String"s].is_string());
        const auto s = ob["String"s];
        require_eq("1234567890"s, s);
    };

    test_case("Double") = [] {
        using xson::json::operator <<;
        auto obj = object{"Double", 21.12};
        auto ss = std::stringstream{};
        ss << obj;
        auto json_str = ss.str();
        succeed(json_str);

        auto ob = json::parse(json_str);
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["Double"s].is_number());
        const auto d = ob["Double"s];
        require_eq(21.12, d);
    };

    test_case("Boolean") = [] {
        using xson::json::operator <<;
        auto obj = object{{"True", true},{"False", false}};
        auto ss = std::stringstream{};
        ss << obj;
        auto json_str = ss.str();
        succeed(json_str);

        auto ob = json::parse(json_str);
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["True"s].is_boolean());
        const auto t = ob["True"s];
        require_eq(true, t);

        require_true(ob["False"s].is_boolean());
        const auto f = ob["False"s];
        require_eq(false, f);
    };

    test_case("Date2String") = [] {
        using xson::json::operator <<;
        auto now = system_clock::now();
        auto obj = object{"Date", now};
        auto ss = std::stringstream{};
        ss << obj;
        auto json_str = ss.str();
        succeed(json_str);

        auto ob = json::parse(json_str);
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["Date"s].is_string());
        const auto d = ob["Date"s];
        require_eq(xson::to_string(now), d);
    };

    test_case("Null") = [] {
        using xson::json::operator <<;
        auto obj = object{"Null", nullptr};
        auto ss = std::stringstream{};
        ss << obj;
        auto json_str = ss.str();
        succeed(json_str);

        auto ob = json::parse(json_str);
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["Null"s].is_null());
    };

    test_case("Int32") = [] {
        using xson::json::operator <<;
        auto obj = object{
            {"Zero", std::int32_t{0}},
            {"Min", std::numeric_limits<int>::min()},
            {"Max", std::numeric_limits<int>::max()}
        };
        auto ss = std::stringstream{};
        ss << obj;
        auto json_str = ss.str();
        succeed(json_str);

        auto ob = json::parse(json_str);
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["Zero"s].is_integer());
        const auto zero = ob["Zero"s];
        require_eq(0, zero);

        require_true(ob["Min"s].is_integer());
        const auto min = ob["Min"s];
        require_eq(std::numeric_limits<int>::min(), min);

        require_true(ob["Max"s].is_integer());
        const auto max = ob["Max"s];
        require_eq(std::numeric_limits<int>::max(), max);
    };

    test_case("Integer") = [] {
        using xson::json::operator <<;
        auto obj = object{
            {"Zero", std::int64_t{0}},
            {"Min", std::numeric_limits<integer_type>::min()},
            {"Max", std::numeric_limits<integer_type>::max()}
        };
        auto ss = std::stringstream{};
        ss << obj;
        auto json_str = ss.str();
        succeed(json_str);

        auto ob = json::parse(json_str);
        ss.str("");
        ss << ob;
        succeed(ss.str());

        require_true(ob["Zero"s].is_integer());
        const xson::integer_type zero = ob["Zero"s];
        require_eq(0, zero);

        require_true(ob["Min"s].is_integer());
        const xson::integer_type min = ob["Min"s];
        require_eq(std::numeric_limits<std::int64_t>::min(), min);

        require_true(ob["Max"s].is_integer());
        const xson::integer_type max = ob["Max"s];
        require_eq(std::numeric_limits<std::int64_t>::max(), max);
    };

    test_case("Array") = [] {
        auto ob1 = xson::object
        {
            {"Array"s, std::array<std::int64_t,3>{1, 2, 3}}
        };
        succeed("ob1:  " + json::stringify(ob1));
        auto str1 = json::stringify(ob1);
        succeed("str1: " + str1);

        auto ob2 = json::parse(str1);
        succeed("ob2:  " + json::stringify(ob2));
        auto str2 = json::stringify(ob2);
        succeed("str2: " + str2);

        check_eq(str1, str2);
    };

    test_case("NestedArrays") = [] {
        auto json_str = R"( {"array":[["a", "b", "c"], [1 ,2, 3], [0.1, 0.2, 0.3]]} )";

        auto ob = json::parse(json_str);
        succeed("ob2:  " + json::stringify(ob));
        auto str2 = json::stringify(ob);
        succeed("str2: " + str2);

        check_eq(static_cast<xson::string_type>(ob["array"s][0][0]), "a"s);
        check_eq(static_cast<xson::string_type>(ob["array"s][0][1]), "b"s);
        check_eq(static_cast<xson::string_type>(ob["array"s][0][2]), "c"s);

        check_eq(static_cast<xson::integer_type>(ob["array"s][1][0]), 1);
        check_eq(static_cast<xson::integer_type>(ob["array"s][1][1]), 2);
        check_eq(static_cast<xson::integer_type>(ob["array"s][1][2]), 3);

        check_eq(static_cast<xson::number_type>(ob["array"s][2][0]), 0.1);
        check_eq(static_cast<xson::number_type>(ob["array"s][2][1]), 0.2);
        check_eq(static_cast<xson::number_type>(ob["array"s][2][2]), 0.3);
    };

    test_case("Vector") = [] {
        auto ob1 = xson::object
        {
            {"Vector"s, std::vector<std::string>{"a", "b", "c", "d", "e", "f"}}
        };
        succeed("ob1:  " + json::stringify(ob1));
        auto str1 = json::stringify(ob1);
        succeed("str1: " + str1);

        auto ob2 = json::parse(str1);
        succeed("ob2:  " + json::stringify(ob2));
        auto str2 = json::stringify(ob2);
        succeed("str2: " + str2);

        check_eq(str1, str2);
    };

    test_case("Complex") = [] {
        using xson::json::operator <<;
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
        succeed(ss.str());

        auto obj = json::parse(ss.str());
        succeed(json::stringify(obj));

        require_true(obj["Ruoka"s].is_boolean());
        const xson::boolean_type b = obj["Ruoka"s];
        require_eq(true, b);

        require_true(obj["Tulppu"s].is_integer());
        const xson::integer_type i1 = obj["Tulppu"s];
        require_eq(1, i1);

        require_true(obj["Ages"s].is_array());

        require_true(obj["Ages"s][4].is_integer());
        const xson::integer_type i2 = obj["Ages"s][4];
        require_eq(2, i2);
    };

    test_case("ParseFile1") = [] {
        const auto loc = std::source_location::current();
        auto source_path = std::filesystem::absolute(std::filesystem::path{loc.file_name()});
        auto test_file = source_path.parent_path().parent_path() / "test" / "xson" / "test1.json";
        
        // Check if file exists
        require_true(std::filesystem::exists(test_file));
        
        auto fs = std::ifstream{test_file};
        require_true(fs.is_open());
        
        auto ob = json::parse(static_cast<std::istream&>(fs));
        succeed("test1.json: "s + json::stringify(ob));

        // Verify the object was parsed (not empty)
        require_true(ob.is_object());
        require_true(ob.size() > 0);

        require_true(ob["required"s].is_array());
        require_true(ob["required"s].size() >= 3);
        require_true(ob["required"s][2].is_string());
        const xson::string_type price = ob["required"s][2];
        check_eq("price"s, price);

        require_true(ob["properties"s].is_object());
        require_true(ob["properties"s]["price"s].is_object());
        require_true(ob["properties"s]["price"s]["minimum"s].is_integer());
        const xson::integer_type minimum = ob["properties"s]["price"s]["minimum"s];
        check_eq(0, minimum);
    };

    test_case("ParseFile2") = [] {
        const auto loc = std::source_location::current();
        auto source_path = std::filesystem::absolute(std::filesystem::path{loc.file_name()});
        auto test_file = source_path.parent_path().parent_path() / "test" / "xson" / "test2.json";
        auto fs = std::ifstream{test_file};
        auto ob = json::parse(static_cast<std::istream&>(fs));
        succeed("test2.json: "s + json::stringify(ob));

        const xson::boolean_type alive = ob["isAlive"s];
        check_eq(true, alive);
        require_true(ob["isAlive"s].is_boolean());

        require_true(ob["spouse"s].is_null());
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_test

