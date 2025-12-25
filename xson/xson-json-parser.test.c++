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

namespace xson::json_parser_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("String") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{"String", "1234567890"s};
        auto ob = json::parse(ss);

        require_true(ob["String"s].is_string());
        const xson::string_type s = ob["String"s];
        require_eq("1234567890"s, s);
    };

    test_case("Double") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{"Double", 21.12};
        auto ob = json::parse(ss);

        require_true(ob["Double"s].is_number());
        const xson::number_type d = ob["Double"s];
        require_eq(21.12, d);
    };

    test_case("Boolean") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{{"True", true},{"False", false}};
        auto ob = json::parse(ss);

        require_true(ob["True"s].is_boolean());
        const xson::boolean_type t = ob["True"s];
        require_eq(true, t);

        require_true(ob["False"s].is_boolean());
        const xson::boolean_type f = ob["False"s];
        require_eq(false, f);
    };

    test_case("Date2String") = [] {
        auto now = system_clock::now();
        auto ss = std::stringstream{};
        ss << xson::object{"Date", now};
        auto ob = json::parse(ss);

        require_true(ob["Date"s].is_string());
        const xson::string_type d = ob["Date"s];
        require_eq(xson::to_string(now), d);
    };

    test_case("Null") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{"Null", nullptr};
        auto ob = json::parse(ss);

        require_true(ob["Null"s].is_null());
    };

    test_case("Int32") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{
            {"Zero", std::int32_t{0}},
            {"Min", std::numeric_limits<int>::min()},
            {"Max", std::numeric_limits<int>::max()}
        };
        auto ob = json::parse(ss);

        require_true(ob["Zero"s].is_integer());
        const xson::integer_type zero = ob["Zero"s];
        require_eq(0, zero);

        require_true(ob["Min"s].is_integer());
        const xson::integer_type min = ob["Min"s];
        require_eq(std::numeric_limits<int>::min(), min);

        require_true(ob["Max"s].is_integer());
        const xson::integer_type max = ob["Max"s];
        require_eq(std::numeric_limits<int>::max(), max);
    };

    test_case("Integer") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{
            {"Zero", std::int64_t{0}},
            {"Min", std::numeric_limits<xson::integer_type>::min()},
            {"Max", std::numeric_limits<xson::integer_type>::max()}
        };
        auto ob = json::parse(ss);

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
        auto str1 = json::stringify(ob1);

        auto ss = std::stringstream{str1};

        auto ob2 = json::parse(ss);
        auto str2 = json::stringify(ob2);

        check_eq(str1, str2);
    };

    test_case("NestedArrays") = [] {
        auto json_str = R"( {"array":[["a", "b", "c"], [1 ,2, 3], [0.1, 0.2, 0.3]]} )";

        auto ob = json::parse(json_str);
        auto str2 = json::stringify(ob);

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
        auto str1 = json::stringify(ob1);

        auto ss = std::stringstream{str1};

        auto ob2 = json::parse(ss);
        auto str2 = json::stringify(ob2);

        check_eq(str1, str2);
    };

    test_case("Complex") = [] {
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

        auto obj = json::parse(ss);

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
        auto fs = std::ifstream{test_file};
        auto ob = json::parse(fs);

        require_true(ob["required"s].is_array());
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
        auto ob = json::parse(fs);

        const xson::boolean_type alive = ob["isAlive"s];
        check_eq(true, alive);
        require_true(ob["isAlive"s].is_boolean());

        require_true(ob["spouse"s].is_null());
    };

    // ===== OVERFLOW/UNDERFLOW TESTS =====

    test_case("LargeIntegerToFloat") = [] {
        // Test number larger than INT64_MAX (should become float)
        auto json_str = R"({"big_num": 9223372036854775808})";
        auto ob = json::parse(json_str);

        require_true(ob["big_num"s].is_number());  // Should be parsed as float due to overflow
        // Just verify it's not parsed as an integer (which would indicate overflow handling failed)
        require_false(ob["big_num"s].is_integer());
    };

    test_case("VeryLargeInteger") = [] {
        // Test extremely large number that definitely exceeds int64
        auto json_str = R"({"huge_num": 999999999999999999999})";
        auto ob = json::parse(json_str);

        require_true(ob["huge_num"s].is_number());  // Should be parsed as float
        require_false(ob["huge_num"s].is_integer());
    };

    test_case("SmallIntegerRemainsInt") = [] {
        // Test that small integers remain as integers
        auto json_str = R"({"small": 42, "zero": 0, "negative": -123})";
        auto ob = json::parse(json_str);

        require_true(ob["small"s].is_integer());
        require_true(ob["zero"s].is_integer());
        require_true(ob["negative"s].is_integer());

        const xson::integer_type small = ob["small"s];
        const xson::integer_type zero = ob["zero"s];
        const xson::integer_type negative = ob["negative"s];

        require_eq(42, small);
        require_eq(0, zero);
        require_eq(-123, negative);
    };

    // ===== ERROR HANDLING TESTS =====

    // test_case("InvalidJSON") = [] {
    //     // Test that malformed JSON throws exceptions
    //     // Test each case individually to ensure exceptions are properly caught
    //     
    //     // Missing closing brace
    //     bool threw1 = false;
    //     try {
    //         auto ob = json::parse(R"({invalid})");
    //     } catch(...) {
    //         threw1 = true;
    //     }
    //     require_true(threw1);
    //     
    //     // Missing quote
    //     bool threw2 = false;
    //     try {
    //         auto ob = json::parse(R"({"unclosed": "string)");
    //     } catch(...) {
    //         threw2 = true;
    //     }
    //     require_true(threw2);
    //     
    //     // Missing closing bracket
    //     bool threw3 = false;
    //     try {
    //         auto ob = json::parse(R"([1,2,3)");
    //     } catch(...) {
    //         threw3 = true;
    //     }
    //     require_true(threw3);
    //     
    //     // Unquoted value
    //     bool threw4 = false;
    //     try {
    //         auto ob = json::parse(R"({"key": value})");
    //     } catch(...) {
    //         threw4 = true;
    //     }
    //     require_true(threw4);
    //     
    //     // Wrong closing bracket
    //     bool threw5 = false;
    //     try {
    //         auto ob = json::parse(R"({"key": 123])");
    //     } catch(...) {
    //         threw5 = true;
    //     }
    //     require_true(threw5);
    //     
    //     // Empty array with just bracket
    //     bool threw6 = false;
    //     try {
    //         auto ob = json::parse(R"([)");
    //     } catch(...) {
    //         threw6 = true;
    //     }
    //     require_true(threw6);
    // };

    test_case("RoundTripConsistency") = [] {
        // Test that parsing -> stringifying -> parsing produces consistent results
        auto original = xson::object{
            {"string", "hello world"s},
            {"integer", 42},
            {"float", 3.14159},
            {"boolean", true},
            {"null", nullptr},
            {"array", std::vector<int>{1, 2, 3}},
            {"nested", xson::object{{"inner", "value"s}}}
        };

        // Round trip: object -> JSON string -> object
        auto json_str = json::stringify(original);
        auto parsed = json::parse(json_str);
        auto json_str2 = json::stringify(parsed);

        // Should be identical
        require_eq(json_str, json_str2);

        // Values should match
        require_eq(static_cast<xson::string_type>(parsed["string"s]), "hello world"s);
        require_eq(static_cast<xson::integer_type>(parsed["integer"s]), 42);
        require_eq(static_cast<xson::number_type>(parsed["float"s]), 3.14159);
        require_true(static_cast<xson::boolean_type>(parsed["boolean"s]));
        require_true(parsed["null"s].is_null());
    };

    // test_case("ScientificNotation") = [] {
    //     // Test parsing of scientific notation - simplified test
    //     auto json_str = R"({"normal": 123, "simple": 1e2})";
    //     auto ob = json::parse(json_str);

    //     // Test normal number first
    //     require_true(ob["normal"s].is_integer());
    //     const xson::integer_type normal = ob["normal"s];
    //     require_eq(123, normal);

    //     // Test scientific notation
    //     require_true(ob["simple"s].is_number());
    //     const xson::number_type value = ob["simple"s];
    //     require_eq(100.0, value);  // 1 * 10^2 = 100
    // };

    test_case("UnicodeAndSpecialChars") = [] {
        // Test handling of Unicode and special characters
        auto json_str = R"({
            "unicode": "Hello 世界 🌍",
            "escapes": "tab:\t newline:\n quote:\" backslash:\\",
            "special": "!@#$%^&*()[]{}|;:,.<>?/~`"
        })";

        auto ob = json::parse(json_str);

        require_true(ob["unicode"s].is_string());
        require_true(ob["escapes"s].is_string());
        require_true(ob["special"s].is_string());

        const xson::string_type unicode = ob["unicode"s];
        const xson::string_type escapes = ob["escapes"s];
        const xson::string_type special = ob["special"s];

        require_eq("Hello 世界 🌍", unicode);
        require_eq("tab:\t newline:\n quote:\" backslash:\\", escapes);
        require_eq("!@#$%^&*()[]{}|;:,.<>?/~`", special);
    };

    test_case("EmptyStructures") = [] {
        // Test empty objects and arrays
        auto json_str = R"({"empty_obj": {}, "empty_arr": []})";
        auto ob = json::parse(json_str);

        require_true(ob["empty_obj"s].is_object());
        require_true(ob["empty_arr"s].is_array());

        require_eq(0u, ob["empty_obj"s].size());
        require_eq(0u, ob["empty_arr"s].size());

        require_true(ob["empty_obj"s].empty());
        require_true(ob["empty_arr"s].empty());
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_parser_test

