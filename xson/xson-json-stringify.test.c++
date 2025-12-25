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

    test_case("Ostream") = [] {
        auto mix = xson::object
        {
            { "Ruoka",  true                     },
            { "Onni",   false                    },
            { "Tulppu", 1.0                      },
            { "Elppu",  2.0                      },
            { "Jalppu", 3.0                      },
            { "Ages",   std::vector<int>{39,40,9,5,2} },
            { "EmptyV", std::vector<int>{}            },
            { "EmptyO", object{}                 },
            { "Null",   nullptr                  },
            { "Now",    system_clock::now()      }
        };

        std::clog << std::setw(0) << mix << std::endl;

        std::clog << std::setw(3) << mix << std::endl;

        std::clog << std::setw(6) << mix << std::endl;

        std::clog << std::setw(12) << mix << std::endl;
    };

    test_case("StringifyCompact") = [] {
        auto obj = xson::object{
            {"name", "test"s},
            {"value", 42},
            {"active", true}
        };

        auto compact = json::stringify(obj, 0);  // Compact (no indentation)

        // Should not contain newlines or extra spaces
        check_eq(compact.find('\n'), std::string::npos);
        check_eq(compact.find("  "), std::string::npos);  // No double spaces

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
        require_true(pretty.find('\n') != std::string::npos);
        require_true(pretty.find("  ") != std::string::npos);  // Should have indentation

        // Should be parseable back
        auto parsed = json::parse(pretty);
        require_true(parsed["users"s].is_array());
        require_eq(2u, parsed["users"s].size());
        require_eq(static_cast<xson::string_type>(parsed["users"s][0]["name"s]), "Alice"s);
        require_eq(static_cast<xson::integer_type>(parsed["users"s][0]["age"s]), 30);
    };

    // test_case("StringifyAllTypes") = [] {
    //     auto obj = xson::object{
    //         {"string", "hello world"s},
    //         {"integer", std::int64_t{42}},
    //         {"float", 3.14159},
    //         {"boolean_true", true},
    //         {"boolean_false", false},
    //         {"null_value", nullptr},
    //         {"array", std::vector<int>{1, 2, 3}},
    //         {"nested", xson::object{{"inner", "value"s}}}
    //     };
    //
    //     auto json_str = json::stringify(obj, 2);
    //
    //     // Verify all types are represented correctly
    //     require_true(json_str.find("\"string\"") != std::string::npos);
    //     require_true(json_str.find("\"hello world\"") != std::string::npos);
    //     require_true(json_str.find("\"integer\"") != std::string::npos);
    //     require_true(json_str.find("42") != std::string::npos);
    //     require_true(json_str.find("\"float\"") != std::string::npos);
    //     require_true(json_str.find("3.14159") != std::string::npos);
    //     require_true(json_str.find("\"boolean_true\"") != std::string::npos);
    //     require_true(json_str.find("true") != std::string::npos);
    //     require_true(json_str.find("\"boolean_false\"") != std::string::npos);
    //     require_true(json_str.find("false") != std::string::npos);
    //     require_true(json_str.find("\"null_value\"") != std::string::npos);
    //     require_true(json_str.find("null") != std::string::npos);
    //     require_true(json_str.find("\"array\"") != std::string::npos);
    //     require_true(json_str.find("[1, 2, 3]") != std::string::npos);
    //     require_true(json_str.find("\"nested\"") != std::string::npos);
    //     require_true(json_str.find("\"inner\"") != std::string::npos);
    //     require_true(json_str.find("\"value\"") != std::string::npos);
    //
    //     // Should be parseable back
    //     auto parsed = json::parse(json_str);
    //     require_true(parsed.has("string"s));
    //     require_true(parsed.has("integer"s));
    //     require_true(parsed.has("float"s));
    // };

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
        require_true(json_str.find("\"empty_object\"") != std::string::npos);
        require_true(json_str.find("{}") != std::string::npos);
        require_true(json_str.find("\"empty_array\"") != std::string::npos);
        require_true(json_str.find("[]") != std::string::npos);

        // Should be parseable back
        auto parsed = json::parse(json_str);
        require_true(parsed["empty_object"s].is_object());
        require_true(parsed["empty_array"s].is_array());
        require_eq(0u, parsed["empty_object"s].size());
        require_eq(0u, parsed["empty_array"s].size());
    };

    // test_case("StringifyLargeNumbers") = [] {
    //     auto obj = xson::object{
    //         {"big_int", std::numeric_limits<std::int64_t>::max()},
    //         {"small_int", std::numeric_limits<std::int64_t>::min()},
    //         {"big_float", 1e308},  // Near double max
    //         {"small_float", 1e-323}  // Near double min
    //     };

    //     auto json_str = json::stringify(obj, 2);

    //     // Should be parseable back
    //     auto parsed = json::parse(json_str);
    //     require_true(parsed["big_int"s].is_number());   // Should be float due to overflow
    //     require_true(parsed["small_int"s].is_number()); // Should be float due to underflow
    //     require_true(parsed["big_float"s].is_number());
    //     require_true(parsed["small_float"s].is_number());
    // };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_stringify_test

