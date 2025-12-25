// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::chrono;
using namespace std::string_literals;
using namespace xson;
using namespace xson::json;

namespace xson::json_encoder_test {

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

    test_case("EncoderCompact") = [] {
        // Test encoder with indent=0 (compact mode) using stringify
        auto obj = xson::object{
            {"string", "hello"s},
            {"number", 42},
            {"array", std::vector<int>{1, 2, 3}},
            {"nested", xson::object{{"inner", "value"s}}}
        };

        auto json_str = json::stringify(obj, 0);

        // Compact mode should have minimal whitespace
        // Should be parseable
        auto parsed = json::parse(json_str);
        require_true(parsed.has("string"s));
        require_eq("hello"s, static_cast<xson::string_type>(parsed["string"s]));
    };

    test_case("EncoderIndentValues") = [] {
        // Test encoder with various indent values using stringify
        auto obj = xson::object{
            {"a", 1},
            {"b", xson::object{{"c", 2}}},
            {"d", std::vector<int>{1, 2, 3}}
        };

        // Test indent=1
        auto json1 = json::stringify(obj, 1);
        require_true(json::parse(json1).has("a"s));

        // Test indent=2 (default)
        auto json2 = json::stringify(obj, 2);
        require_true(json::parse(json2).has("a"s));

        // Test indent=4
        auto json4 = json::stringify(obj, 4);
        require_true(json::parse(json4).has("a"s));

        // Test indent=8
        auto json8 = json::stringify(obj, 8);
        require_true(json::parse(json8).has("a"s));
    };

    test_case("EncoderEmptyStructures") = [] {
        // Test encoder with empty objects and arrays
        auto obj = xson::object{
            {"empty_obj", xson::object{}},
            {"empty_arr", xson::object::array{}}
        };

        auto json_str = json::stringify(obj, 2);

        // Should contain empty structures
        require_true(json_str.find("empty_obj") != std::string::npos);
        require_true(json_str.find("empty_arr") != std::string::npos);

        // Should be parseable
        auto parsed = json::parse(json_str);
        require_true(parsed["empty_obj"s].is_object());
        require_true(parsed["empty_obj"s].empty());
        require_true(parsed["empty_arr"s].is_array());
        require_true(parsed["empty_arr"s].empty());
    };

    test_case("EncoderNestedStructures") = [] {
        // Test encoder with deeply nested structures
        auto obj = xson::object{
            {"level1", xson::object{
                {"level2", xson::object{
                    {"level3", xson::object{
                        {"value", 42}
                    }}
                }}
            }},
            {"array", std::vector<xson::object>{
                xson::object{{"a", 1}},
                xson::object{{"b", 2}},
                xson::object{{"c", xson::object{{"d", 3}}}}
            }}
        };

        auto json_str = json::stringify(obj, 2);

        // Should be parseable
        auto parsed = json::parse(json_str);
        require_true(parsed.has("level1"s));
        require_true(parsed["level1"s].has("level2"s));
        require_true(parsed["level1"s]["level2"s].has("level3"s));
        require_eq(42, static_cast<xson::integer_type>(parsed["level1"s]["level2"s]["level3"s]["value"s]));
        
        require_true(parsed["array"s].is_array());
        require_eq(3u, parsed["array"s].size());
    };

    test_case("EncoderRoundTrip") = [] {
        // Test that encoder output can be parsed and re-encoded consistently
        auto original = xson::object{
            {"string", "test"s},
            {"integer", 123},
            {"float", 3.14},
            {"boolean", true},
            {"null", nullptr},
            {"array", std::vector<int>{1, 2, 3}},
            {"object", xson::object{{"nested", "value"s}}}
        };

        // Encode with indent=2
        auto json1 = json::stringify(original, 2);

        // Parse and re-encode
        auto parsed = json::parse(json1);
        auto json2 = json::stringify(parsed, 2);

        // Parse both and compare
        auto parsed1 = json::parse(json1);
        auto parsed2 = json::parse(json2);
        
        require_eq(parsed1["string"s], parsed2["string"s]);
        require_eq(parsed1["integer"s], parsed2["integer"s]);
        require_eq(parsed1["float"s], parsed2["float"s]);
        require_eq(parsed1["boolean"s], parsed2["boolean"s]);
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_encoder_test

