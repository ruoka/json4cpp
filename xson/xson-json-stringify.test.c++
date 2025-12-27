// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::chrono;
using namespace std::string_literals;
using namespace xson;
using namespace xson::json;

namespace xson::json_stringify_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("StringifyCompact") = [] {
        auto obj = xson::object{
            {"name", "test"s},
            {"value", 42},
            {"active", true}
        };

        auto compact = json::stringify(obj, 0);  // Compact (no indentation)

        // Should not contain newlines or extra spaces
        check_false(compact.contains('\n'));
        check_false(compact.contains("  "));  // No double spaces

        // Should be parseable back
        auto parsed = json::parse(compact);
        require_eq(static_cast<xson::string_type>(parsed["name"s]), "test"s);
        require_eq(static_cast<xson::integer_type>(parsed["value"s]), 42);
        require_true(static_cast<xson::boolean_type>(parsed["active"s]));
    };

    test_case("StringifyPretty") = [] {
        auto obj = xson::object{
            {"users", std::vector<xson::object>{
                {{"name", "Alice"s}, {"age", 30}},
                {{"name", "Bob"s}, {"age", 25}}
            }},
            {"metadata", xson::object{
                {"version", "1.0"s},
                {"count", 2}
            }}
        };

        auto pretty = json::stringify(obj, 2);  // Pretty print with 2-space indent

        // Should contain newlines and proper indentation
        require_contains(pretty, '\n');
        require_contains(pretty, "  ");  // Should have indentation

        // Should be parseable back
        auto parsed = json::parse(pretty);
        require_true(parsed["users"s].is_array());
        require_eq(2u, parsed["users"s].size());
        require_eq(static_cast<xson::string_type>(parsed["users"s][0]["name"s]), "Alice"s);
        require_eq(static_cast<xson::integer_type>(parsed["users"s][0]["age"s]), 30);
    };

    test_case("StringifyAllTypes") = [] {
        auto obj = xson::object{
            {"string", "hello world"s},
            {"integer", std::int64_t{42}},
            {"float", 3.14159},
            {"boolean_true", true},
            {"boolean_false", false},
            {"null_value", nullptr},
            {"array", std::vector<int>{1, 2, 3}},
            {"nested", xson::object{{"inner", "value"s}}}
        };

        auto json_str = json::stringify(obj, 2);

        // Verify all types are represented correctly
        require_contains(json_str, "\"string\"");
        require_contains(json_str, "\"hello world\"");
        require_contains(json_str, "\"integer\"");
        require_contains(json_str, "42");
        require_contains(json_str, "\"float\"");
        require_contains(json_str, "3.14159");
        require_contains(json_str, "\"boolean_true\"");
        require_contains(json_str, "true");
        require_contains(json_str, "\"boolean_false\"");
        require_contains(json_str, "false");
        require_contains(json_str, "\"null_value\"");
        require_contains(json_str, "null");
        require_contains(json_str, "\"array\"");
        require_contains(json_str, "\"nested\"");
        require_contains(json_str, "\"inner\"");
        require_contains(json_str, "\"value\"");

        // Parse and verify all content (more robust than string matching)
        auto parsed = json::parse(json_str);
        // Verify array content
        require_true(parsed.has("array"s));
        require_true(parsed["array"s].is_array());
        const auto& arr = parsed["array"s].get<xson::object::array>();
        std::vector<int> arr_values;
        arr_values.reserve(arr.size());
        for (const auto& val : arr) {
            arr_values.push_back(static_cast<int>(val));
        }
        require_container_eq(arr_values, std::vector<int>{1, 2, 3});
        require_true(parsed.has("string"s));
        require_true(parsed.has("integer"s));
        require_true(parsed.has("float"s));
    };

    test_case("StringifyRootPrimitives") = [] {
        // Stringify should handle non-container top-level values too.
        {
            auto v = xson::object{};
            v = true;
            const auto s = json::stringify(v, 0);
            require_eq("true"s, s);
            const auto parsed = json::parse(s);
            require_true(parsed.is_boolean());
            require_true(static_cast<xson::boolean_type>(parsed));
        }
        {
            auto v = xson::object{};
            v = nullptr;
            const auto s = json::stringify(v, 0);
            require_eq("null"s, s);
            const auto parsed = json::parse(s);
            require_true(parsed.is_null());
        }
        {
            auto v = xson::object{};
            v = std::int64_t{42};
            const auto s = json::stringify(v, 0);
            require_eq("42"s, s);
            const auto parsed = json::parse(s);
            require_true(parsed.is_integer());
            require_eq(42, static_cast<xson::integer_type>(parsed));
        }
        {
            auto v = xson::object{};
            v = 1.25;
            const auto s = json::stringify(v, 0);
            require_true(!s.empty());
            require_false(s.starts_with('{') || s.starts_with('['));
            const auto parsed = json::parse(s);
            require_true(parsed.is_number());
            require_eq(1.25, static_cast<xson::number_type>(parsed));
        }
        {
            auto v = xson::object{};
            v = "hi"s;
            const auto s = json::stringify(v, 0);
            require_eq("\"hi\""s, s);
            const auto parsed = json::parse(s);
            require_true(parsed.is_string());
            require_eq("hi"s, static_cast<xson::string_type>(parsed));
        }
    };

    test_case("StringifyEmptyStructures") = [] {
        auto obj = xson::object{
            {"empty_object", xson::object{}},
            {"empty_array", std::vector<int>{}},
            {"nested_empty", xson::object{
                {"empty_obj", xson::object{}},
                {"empty_arr", std::vector<std::string>{}}
            }}
        };

        auto json_str = json::stringify(obj, 2);

        // Should contain empty objects and arrays
        require_contains(json_str, "\"empty_object\"");
        require_contains(json_str, "{}");
        require_contains(json_str, "\"empty_array\"");
        require_contains(json_str, "[]");

        // Should be parseable back
        auto parsed = json::parse(json_str);
        require_true(parsed["empty_object"s].is_object());
        require_true(parsed["empty_array"s].is_array());
        require_eq(0u, parsed["empty_object"s].size());
        require_eq(0u, parsed["empty_array"s].size());
    };

    test_case("StringifyLargeNumbers") = [] {
        auto obj = xson::object{
            {"big_int", std::numeric_limits<std::int64_t>::max()},
            {"small_int", std::numeric_limits<std::int64_t>::min()},
            {"large_float", 123456789.123456789},  // Large but not scientific notation
            {"small_float", 0.000000123456789}     // Small but not scientific notation
        };

        auto json_str = json::stringify(obj, 2);

        // Verify the stringified output contains the keys
        require_contains(json_str, "big_int");
        require_contains(json_str, "small_int");
        require_contains(json_str, "large_float");
        require_contains(json_str, "small_float");
        
        // Should be parseable back
        auto parsed = json::parse(json_str);
        require_true(parsed["big_int"s].is_integer());   // INT64_MAX should be integer
        require_true(parsed["small_int"s].is_integer()); // INT64_MIN should be integer
        require_true(parsed["large_float"s].is_number());
        require_true(parsed["small_float"s].is_number());
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_stringify_test

