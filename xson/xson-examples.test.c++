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

        auto ss = std::stringstream{R"(
            {
                "id" : 2,
                "Name" : "Ruoka",
                "Embedded" : {
                    "id" : 5,
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

        auto result = json::parse(ss.str());

        std::clog << xson::json::stringify(result, 2) << "\n\n";

        std::clog << "_id            = " << result["id"s]                << "\n"
                 << "Name           = " << result["Name"s]              << "\n"
                 << "Embeded.Name   = " << result["Embedded"s]["Name"s] << "\n"
                 << "Lucky Number 1 = " << result["Lucky Numbers"s][0]  << "\n"
                 << "Lucky Number 2 = " << result["Lucky Numbers"s][1]  << "\n"
                 << "Lucky Number 3 = " << result["Lucky Numbers"s][2]  << "\n\n";

        [[maybe_unused]] integer_type id = result["id"s];

        [[maybe_unused]] string_type name = result["Name"s];

        [[maybe_unused]] integer_type number = result["Lucky Numbers"s][1];
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::examples_test

