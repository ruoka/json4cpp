// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::string_literals;
using namespace xson;


namespace xson::examples_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("ReadmeCreateStringify") = [] {
        // Mirrors README "Create + stringify" snippet.
        auto document = object{
            { "Name"s, "Papa"s },
            { "Age"s,  40      },
            { "LuckyNumbers"s, {2, 22, 2112} },
            { "Lucky"s, false }
        };

        const auto json_str = xson::json::stringify(document);
        const auto parsed = xson::json::parse(json_str);

        require_true(parsed.is_object());
        require_eq("Papa"s, static_cast<string_type>(parsed["Name"s]));
        require_eq(40, static_cast<integer_type>(parsed["Age"s]));
        require_false(static_cast<boolean_type>(parsed["Lucky"s]));
        require_true(parsed["LuckyNumbers"s].is_array());
        require_eq(3u, parsed["LuckyNumbers"s].size());
        require_eq(22, static_cast<integer_type>(parsed["LuckyNumbers"s][1]));
    };

    test_case("Stringify") = [] {
        auto kids = std::vector<object>
        {
            { "Name"s, "Tulppu"s },
            { "Name"s, "Elppu"s  },
            { "Name"s, "Jalppu"s }
        };

        auto sizes = object
        {
            { "Height"s,   200   },
            { "Waist"s,    120.5 },
            { "ShoeSize"s, 47.5  }
        };

        succeed("X"s);

        auto papa = object
        {
            { "Name",         "Papa"s       },
            { "Age",          40            },
            { "Kids",         kids          },
            { "Measures",     sizes         },
            { "LuckyNumbers", {2, 22, 2112} },
            { "Lucky",        false         }
        };

        std::clog << xson::json::stringify(papa, 2) << std::endl;
    };

    test_case("Parse") = [] {
        using namespace xson::json;
        using xson::json::operator >>;

        auto ss = std::stringstream{R"(
            {
                "_id" : 2,
                "Name" : "Ruoka",
                "Embedded" : {
                    "_id" : 5,
                    "Name" : "Tuma"
                },
                "Lucky Numbers" : [
                    2,
                    22,
                    2112
                ]
            }
        )"};

        std::clog << ss.str() << "\n\n";

        auto result = json::parse(static_cast<std::istream&>(ss));

        std::clog << xson::json::stringify(result, 2) << "\n\n";

        std::clog << "_id            = " << result["_id"s]               << "\n"
                 << "Name           = " << result["Name"s]              << "\n"
                 << "Embeded.Name   = " << result["Embedded"s]["Name"s] << "\n"
                 << "Lucky Number 1 = " << result["Lucky Numbers"s][0]  << "\n"
                 << "Lucky Number 2 = " << result["Lucky Numbers"s][1]  << "\n"
                 << "Lucky Number 3 = " << result["Lucky Numbers"s][2]  << "\n\n";

        // Add assertions to verify parsing worked correctly
        require_true(result.is_object());
        require_true(result["_id"s].is_integer());
        require_eq(2, static_cast<integer_type>(result["_id"s]));
        
        require_true(result["Name"s].is_string());
        require_eq("Ruoka"s, static_cast<string_type>(result["Name"s]));
        
        require_true(result["Embedded"s].is_object());
        require_true(result["Embedded"s]["Name"s].is_string());
        require_eq("Tuma"s, static_cast<string_type>(result["Embedded"s]["Name"s]));
        
        require_true(result["Lucky Numbers"s].is_array());
        require_eq(3, result["Lucky Numbers"s].size());
        require_eq(2, static_cast<integer_type>(result["Lucky Numbers"s][0]));
        require_eq(22, static_cast<integer_type>(result["Lucky Numbers"s][1]));
        require_eq(2112, static_cast<integer_type>(result["Lucky Numbers"s][2]));

        // Also validate the stream operator>> form (README example).
        auto ss2 = std::stringstream{R"(
            {
                "_id" : 2,
                "Name" : "Ruoka",
                "Embedded" : {
                    "_id" : 5,
                    "Name" : "Tuma"
                },
                "Lucky Numbers" : [
                    2,
                    22,
                    2112
                ]
            }
        )"};
        auto result2 = xson::object{};
        ss2 >> result2;
        require_eq(result, result2);
    };

    test_case("RootValues") = [] {
        using namespace xson::json;

        const auto v1 = json::parse("true");
        require_true(v1.is_boolean());
        require_true(static_cast<boolean_type>(v1));

        const auto v2 = json::parse("42");
        require_true(v2.is_integer());
        require_eq(42, static_cast<integer_type>(v2));

        const auto v3 = json::parse("\"hello\"");
        require_true(v3.is_string());
        require_eq("hello"s, static_cast<string_type>(v3));

        // Ensure stringify round-trip is stable for root values.
        require_eq("true"s, json::stringify(v1, 0));
        require_eq("42"s, json::stringify(v2, 0));
        require_eq("\"hello\""s, json::stringify(v3, 0));
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::examples_test

