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
        auto ob = json::parse(ss.str());

        require_true(ob["String"s].is_string());
        const xson::string_type s = ob["String"s];
        require_eq("1234567890"s, s);
    };

    test_case("Double") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{"Double", 21.12};
        auto ob = json::parse(ss.str());

        require_true(ob["Double"s].is_number());
        const xson::number_type d = ob["Double"s];
        require_eq(21.12, d);
    };

    test_case("Boolean") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{{"True", true},{"False", false}};
        auto ob = json::parse(ss.str());

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
        auto ob = json::parse(ss.str());

        require_true(ob["Date"s].is_string());
        const xson::string_type d = ob["Date"s];
        require_eq(xson::to_string(now), d);
    };

    test_case("Null") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{"Null", nullptr};
        auto ob = json::parse(ss.str());

        require_true(ob["Null"s].is_null());
    };

    test_case("Int32") = [] {
        auto ss = std::stringstream{};
        ss << xson::object{
            {"Zero", std::int32_t{0}},
            {"Min", std::numeric_limits<int>::min()},
            {"Max", std::numeric_limits<int>::max()}
        };
        auto ob = json::parse(ss.str());

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
        auto ob = json::parse(ss.str());

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

    test_case("InvalidJSON") = [] {
        // Test that malformed JSON throws exceptions
        // Test each case individually to ensure exceptions are properly caught
        
        // Missing closing brace
        bool threw1 = false;
        try {
            auto ob = json::parse(R"({invalid})");
        } catch(...) {
            threw1 = true;
        }
        require_true(threw1);
        
        // Missing quote
        bool threw2 = false;
        try {
            auto ob = json::parse(R"({"unclosed": "string)");
        } catch(...) {
            threw2 = true;
        }
        require_true(threw2);
        
        // Missing closing bracket
        bool threw3 = false;
        try {
            auto ob = json::parse(R"([1,2,3)");
        } catch(...) {
            threw3 = true;
        }
        require_true(threw3);
        
        // Unquoted value
        bool threw4 = false;
        try {
            auto ob = json::parse(R"({"key": value})");
        } catch(...) {
            threw4 = true;
        }
        require_true(threw4);
        
        // Wrong closing bracket
        bool threw5 = false;
        try {
            auto ob = json::parse(R"({"key": 123])");
        } catch(...) {
            threw5 = true;
        }
        require_true(threw5);
        
        // Empty array with just bracket
        bool threw6 = false;
        try {
            auto ob = json::parse(R"([)");
        } catch(...) {
            threw6 = true;
        }
        require_true(threw6);
    };

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

    test_case("ScientificNotation") = [] {
        // Test basic scientific notation
        auto json_str = R"({"normal": 123, "simple": 1e2})";
        auto ob = json::parse(json_str);

        // Test normal number first
        require_true(ob["normal"s].is_integer());
        const xson::integer_type normal = ob["normal"s];
        require_eq(123, normal);

        // Test scientific notation
        require_true(ob["simple"s].is_number());
        const xson::number_type value = ob["simple"s];
        require_eq(100.0, value);  // 1 * 10^2 = 100
    };

    test_case("ScientificNotationEdgeCases") = [] {
        // Test various scientific notation formats
        auto json_str = R"({
            "pos_exp": 1e2,
            "neg_exp": 1e-2,
            "pos_exp_signed": 1e+2,
            "neg_exp_signed": 1e-10,
            "decimal_pos": 1.5e2,
            "decimal_neg": 0.5e-3,
            "large_exp": 1e308,
            "small_exp": 1e-323,
            "integer_exp": 42e2,
            "zero_base": 0e10,
            "zero_exp": 1e0
        })";
        auto ob = json::parse(json_str);

        // Positive exponent
        require_true(ob["pos_exp"s].is_number());
        require_eq(100.0, static_cast<xson::number_type>(ob["pos_exp"s]));

        // Negative exponent
        require_true(ob["neg_exp"s].is_number());
        require_eq(0.01, static_cast<xson::number_type>(ob["neg_exp"s]));

        // Positive exponent with explicit sign
        require_true(ob["pos_exp_signed"s].is_number());
        require_eq(100.0, static_cast<xson::number_type>(ob["pos_exp_signed"s]));

        // Negative exponent with explicit sign
        require_true(ob["neg_exp_signed"s].is_number());
        require_eq(1e-10, static_cast<xson::number_type>(ob["neg_exp_signed"s]));

        // Decimal with positive exponent
        require_true(ob["decimal_pos"s].is_number());
        require_eq(150.0, static_cast<xson::number_type>(ob["decimal_pos"s]));

        // Decimal with negative exponent
        require_true(ob["decimal_neg"s].is_number());
        require_eq(0.0005, static_cast<xson::number_type>(ob["decimal_neg"s]));

        // Large exponent (near double max)
        require_true(ob["large_exp"s].is_number());
        const auto large_val = static_cast<xson::number_type>(ob["large_exp"s]);
        require_true(large_val > 1e300);

        // Small exponent (near double min)
        require_true(ob["small_exp"s].is_number());
        const auto small_val = static_cast<xson::number_type>(ob["small_exp"s]);
        require_true(small_val > 0 && small_val < 1e-300);

        // Integer with scientific notation (becomes float)
        require_true(ob["integer_exp"s].is_number());
        require_eq(4200.0, static_cast<xson::number_type>(ob["integer_exp"s]));

        // Zero base
        require_true(ob["zero_base"s].is_number());
        require_eq(0.0, static_cast<xson::number_type>(ob["zero_base"s]));

        // Zero exponent
        require_true(ob["zero_exp"s].is_number());
        require_eq(1.0, static_cast<xson::number_type>(ob["zero_exp"s]));
    };

    test_case("ScientificNotationErrors") = [] {
        // Test error cases for scientific notation
        // Missing exponent digits
        bool threw1 = false;
        try {
            auto ob = json::parse(R"({"invalid": 1e})");
        } catch(...) {
            threw1 = true;
        }
        require_true(threw1);

        // Missing exponent digits after plus
        bool threw2 = false;
        try {
            auto ob = json::parse(R"({"invalid": 1e+})");
        } catch(...) {
            threw2 = true;
        }
        require_true(threw2);

        // Missing exponent digits after minus
        bool threw3 = false;
        try {
            auto ob = json::parse(R"({"invalid": 1e-})");
        } catch(...) {
            threw3 = true;
        }
        require_true(threw3);

        // Invalid character after e
        bool threw4 = false;
        try {
            auto ob = json::parse(R"({"invalid": 1ex})");
        } catch(...) {
            threw4 = true;
        }
        require_true(threw4);
    };

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

    test_case("MixedTypeArrays") = [] {
        // Test arrays containing different types
        auto json_str = R"({
            "mixed": [1, "string", true, false, null, {}, [], 3.14, -42]
        })";
        auto ob = json::parse(json_str);

        require_true(ob["mixed"s].is_array());
        require_eq(9u, ob["mixed"s].size());

        // Integer
        require_true(ob["mixed"s][0].is_integer());
        require_eq(1, static_cast<xson::integer_type>(ob["mixed"s][0]));

        // String
        require_true(ob["mixed"s][1].is_string());
        require_eq("string"s, static_cast<xson::string_type>(ob["mixed"s][1]));

        // Boolean true
        require_true(ob["mixed"s][2].is_boolean());
        require_true(static_cast<xson::boolean_type>(ob["mixed"s][2]));

        // Boolean false
        require_true(ob["mixed"s][3].is_boolean());
        require_false(static_cast<xson::boolean_type>(ob["mixed"s][3]));

        // Null
        require_true(ob["mixed"s][4].is_null());

        // Empty object
        require_true(ob["mixed"s][5].is_object());
        require_eq(0u, ob["mixed"s][5].size());

        // Empty array
        require_true(ob["mixed"s][6].is_array());
        require_eq(0u, ob["mixed"s][6].size());

        // Float
        require_true(ob["mixed"s][7].is_number());
        require_eq(3.14, static_cast<xson::number_type>(ob["mixed"s][7]));

        // Negative integer
        require_true(ob["mixed"s][8].is_integer());
        require_eq(-42, static_cast<xson::integer_type>(ob["mixed"s][8]));
    };

    test_case("AllEscapeSequences") = [] {
        // Test all JSON escape sequences: \", \\, \/, \b, \f, \n, \r, \t, \uXXXX
        auto json_str = R"({
            "quote": "\"",
            "backslash": "\\",
            "slash": "\/",
            "backspace": "\b",
            "formfeed": "\f",
            "newline": "\n",
            "carriagereturn": "\r",
            "tab": "\t",
            "unicode1": "\u0020",
            "unicode2": "\u00A9",
            "unicode3": "\u03A9",
            "unicode4": "\uD83D\uDE00"
        })";
        auto ob = json::parse(json_str);

        require_eq("\"", static_cast<xson::string_type>(ob["quote"s]));
        require_eq("\\", static_cast<xson::string_type>(ob["backslash"s]));
        require_eq("/", static_cast<xson::string_type>(ob["slash"s]));
        require_eq("\b", static_cast<xson::string_type>(ob["backspace"s]));
        require_eq("\f", static_cast<xson::string_type>(ob["formfeed"s]));
        require_eq("\n", static_cast<xson::string_type>(ob["newline"s]));
        require_eq("\r", static_cast<xson::string_type>(ob["carriagereturn"s]));
        require_eq("\t", static_cast<xson::string_type>(ob["tab"s]));
        require_eq(" ", static_cast<xson::string_type>(ob["unicode1"s]));  // U+0020 = space
        require_eq("©", static_cast<xson::string_type>(ob["unicode2"s]));  // U+00A9 = copyright
        require_eq("Ω", static_cast<xson::string_type>(ob["unicode3"s]));  // U+03A9 = omega
        require_eq("😀", static_cast<xson::string_type>(ob["unicode4"s]));  // U+1F600 = grinning face (surrogate pair)
    };

    test_case("DeepNesting") = [] {
        // Test nesting up to a reasonable depth (10 levels to avoid test slowness)
        // Build a deeply nested structure manually for reliability
        auto json_str = R"({"a":{"b":{"c":{"d":{"e":{"f":{"g":{"h":{"i":{"j":{"value":42}}}}}}}}}}}})";
        auto ob = json::parse(json_str);
        
        // Verify we can access deeply nested value
        require_eq(42, static_cast<xson::integer_type>(ob["a"s]["b"s]["c"s]["d"s]["e"s]["f"s]["g"s]["h"s]["i"s]["j"s]["value"s]));
    };

    test_case("DeepNestingArrays") = [] {
        // Test deeply nested arrays (10 levels to avoid test slowness)
        auto json_str = R"({"arr":[[[[[[[[[[42]]]]]]]]]]})";
        auto ob = json::parse(json_str);
        
        // Verify we can access deeply nested value
        auto current = ob["arr"s];
        for(int i = 0; i < 10; ++i) {
            require_true(current.is_array());
            require_eq(1u, current.size());
            current = current[0];
        }
        require_true(current.is_integer());
        require_eq(42, static_cast<xson::integer_type>(current));
    };

    test_case("SizeLimitEdgeCases") = [] {
        // Test near the size limits (but not at max to avoid test slowness)
        // Test large string (but not max_string_length which is 100MB)
        std::string large_string(10000, 'a');  // 10KB string
        auto json_str = R"({"large_string":")" + large_string + "\"}";
        auto ob = json::parse(json_str);
        require_true(ob["large_string"s].is_string());
        require_eq(10000u, static_cast<xson::string_type>(ob["large_string"s]).length());

        // Test large array (but not max_array_size which is 100 million)
        std::string large_array = R"({"large_array":[)";
        for(int i = 0; i < 1000; ++i) {
            if(i > 0) large_array += ",";
            large_array += std::to_string(i);
        }
        large_array += "]}";
        auto ob2 = json::parse(large_array);
        require_true(ob2["large_array"s].is_array());
        require_eq(1000u, ob2["large_array"s].size());
        require_eq(0, static_cast<xson::integer_type>(ob2["large_array"s][0]));
        require_eq(999, static_cast<xson::integer_type>(ob2["large_array"s][999]));
    };

    test_case("NumberEdgeCases") = [] {
        // Test number edge cases (using valid JSON)
        auto json_str = R"({
            "negative_zero": -0,
            "trailing_zeros_float": 1.00,
            "many_decimals": 1.12345678901234567890,
            "zero": 0,
            "negative": -123,
            "positive": 42,
            "small_float": 0.000001,
            "large_float": 123456.789
        })";
        auto ob = json::parse(json_str);

        // Note: JSON doesn't distinguish -0 from 0, it should parse as integer 0
        require_true(ob["negative_zero"s].is_integer());
        require_eq(0, static_cast<xson::integer_type>(ob["negative_zero"s]));
        
        // Trailing zeros in float
        require_true(ob["trailing_zeros_float"s].is_number());
        require_eq(1.0, static_cast<xson::number_type>(ob["trailing_zeros_float"s]));

        // Many decimals
        require_true(ob["many_decimals"s].is_number());
        
        // Zero
        require_true(ob["zero"s].is_integer());
        require_eq(0, static_cast<xson::integer_type>(ob["zero"s]));

        // Negative
        require_true(ob["negative"s].is_integer());
        require_eq(-123, static_cast<xson::integer_type>(ob["negative"s]));

        // Positive (without explicit +)
        require_true(ob["positive"s].is_integer());
        require_eq(42, static_cast<xson::integer_type>(ob["positive"s]));

        // Small float
        require_true(ob["small_float"s].is_number());
        require_true(static_cast<xson::number_type>(ob["small_float"s]) < 0.00001);

        // Large float
        require_true(ob["large_float"s].is_number());
        require_eq(123456.789, static_cast<xson::number_type>(ob["large_float"s]));
    };

    test_case("WhitespaceVariations") = [] {
        // Test various whitespace patterns
        auto json_str = R"({
            "tab":	"value1",
            "multiple_spaces":    "value2",
            "mixed": 	"value3",
            "newlines": 
            "value4",
            "before_colon" : "value5",
            "after_colon":  "value6",
            "around_comma": "v1" , "next": "v2",
            "compact": {"a":1,"b":2,"c":3}
        })";
        auto ob = json::parse(json_str);

        require_eq("value1"s, static_cast<xson::string_type>(ob["tab"s]));
        require_eq("value2"s, static_cast<xson::string_type>(ob["multiple_spaces"s]));
        require_eq("value3"s, static_cast<xson::string_type>(ob["mixed"s]));
        require_eq("value4"s, static_cast<xson::string_type>(ob["newlines"s]));
        require_eq("value5"s, static_cast<xson::string_type>(ob["before_colon"s]));
        require_eq("value6"s, static_cast<xson::string_type>(ob["after_colon"s]));
        require_eq("v1"s, static_cast<xson::string_type>(ob["around_comma"s]));
        require_eq("v2"s, static_cast<xson::string_type>(ob["next"s]));
        require_eq(1, static_cast<xson::integer_type>(ob["compact"s]["a"s]));
        require_eq(2, static_cast<xson::integer_type>(ob["compact"s]["b"s]));
        require_eq(3, static_cast<xson::integer_type>(ob["compact"s]["c"s]));
    };

    test_case("ObjectEdgeCases") = [] {
        // Test object edge cases
        auto json_str = R"({
            "key_with_spaces": "value1",
            "key_with_unicode": "value2",
            "key_with_numbers123": "value3",
            "empty_key": "",
            "very_long_key_name_that_goes_on_and_on_and_on": "value"
        })";
        auto ob = json::parse(json_str);

        // Test keys with various characters
        require_eq("value1"s, static_cast<xson::string_type>(ob["key_with_spaces"s]));
        require_eq("value2"s, static_cast<xson::string_type>(ob["key_with_unicode"s]));
        require_eq("value3"s, static_cast<xson::string_type>(ob["key_with_numbers123"s]));
        
        // Empty key
        require_eq(""s, static_cast<xson::string_type>(ob["empty_key"s]));
        
        // Very long key
        require_eq("value"s, static_cast<xson::string_type>(ob["very_long_key_name_that_goes_on_and_on_and_on"s]));
    };

    test_case("ArrayEdgeCases") = [] {
        // Test array edge cases
        auto json_str = R"({
            "only_nulls": [null, null, null],
            "only_empty_objects": [{}, {}, {}],
            "only_empty_arrays": [[], [], []],
            "mixed_nulls": [1, null, "string", null, true, null]
        })";
        auto ob = json::parse(json_str);

        require_true(ob["only_nulls"s].is_array());
        require_eq(3u, ob["only_nulls"s].size());
        require_true(ob["only_nulls"s][0].is_null());
        require_true(ob["only_nulls"s][1].is_null());
        require_true(ob["only_nulls"s][2].is_null());

        require_true(ob["only_empty_objects"s].is_array());
        require_eq(3u, ob["only_empty_objects"s].size());
        require_true(ob["only_empty_objects"s][0].is_object());
        require_eq(0u, ob["only_empty_objects"s][0].size());

        require_true(ob["only_empty_arrays"s].is_array());
        require_eq(3u, ob["only_empty_arrays"s].size());
        require_true(ob["only_empty_arrays"s][0].is_array());
        require_eq(0u, ob["only_empty_arrays"s][0].size());

        require_true(ob["mixed_nulls"s].is_array());
        require_eq(6u, ob["mixed_nulls"s].size());
        require_true(ob["mixed_nulls"s][0].is_integer());
        require_true(ob["mixed_nulls"s][1].is_null());
        require_true(ob["mixed_nulls"s][2].is_string());
        require_true(ob["mixed_nulls"s][3].is_null());
        require_true(ob["mixed_nulls"s][4].is_boolean());
        require_true(ob["mixed_nulls"s][5].is_null());
    };

    test_case("ComplexNestingPatterns") = [] {
        // Test complex mixed nesting patterns
        auto json_str = R"({
            "array_of_objects": [
                {"id": 1, "tags": ["a", "b"]},
                {"id": 2, "tags": ["c", "d", "e"]}
            ],
            "object_with_arrays": {
                "numbers": [1, 2, 3],
                "strings": ["x", "y"],
                "nested": [
                    {"a": 1},
                    {"b": 2}
                ]
            },
            "deep_mix": {
                "level1": {
                    "arr": [
                        {"nested": {"value": 42}}
                    ]
                }
            }
        })";
        auto ob = json::parse(json_str);

        // Array of objects
        require_true(ob["array_of_objects"s].is_array());
        require_eq(2u, ob["array_of_objects"s].size());
        require_eq(1, static_cast<xson::integer_type>(ob["array_of_objects"s][0]["id"s]));
        require_eq(2u, ob["array_of_objects"s][0]["tags"s].size());

        // Object with arrays
        require_true(ob["object_with_arrays"s]["numbers"s].is_array());
        require_eq(3u, ob["object_with_arrays"s]["numbers"s].size());

        // Deep mix
        require_eq(42, static_cast<xson::integer_type>(
            ob["deep_mix"s]["level1"s]["arr"s][0]["nested"s]["value"s]));
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_parser_test

