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
        for (const auto indent : {0, 3, 6, 12}) {
            auto ss = std::stringstream{};
            ss << std::setw(indent) << mix;
            const auto out = ss.str();
            require_true(!out.empty());

            // Encoder output must always be parseable JSON.
            const auto parsed = json::parse(out);
            require_true(parsed["Ruoka"s].is_boolean());
            require_true(parsed["Ages"s].is_array());
        }
    };

    test_case("OstreamRootPrimitives") = [] {
        // Stream encoder should also handle non-container top-level values.
        for (const auto indent : {0, 4}) {
            {
                auto v = xson::object{};
                v = true;
                auto ss = std::stringstream{};
                ss << std::setw(indent) << v;
                require_eq("true"s, ss.str());
            }
            {
                auto v = xson::object{};
                v = "hi"s;
                auto ss = std::stringstream{};
                ss << std::setw(indent) << v;
                const auto out = ss.str();
                const auto parsed = json::parse(out);
                require_true(parsed.is_string());
                require_eq("hi"s, static_cast<xson::string_type>(parsed));
            }
        }
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::json_encoder_test

