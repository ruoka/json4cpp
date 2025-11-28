// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::string_literals;
using namespace xson::fast;

namespace xson::fast_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("Byte") = [] {
        auto ss = std::stringstream{};

        auto b1 = std::uint8_t{1};
        xson::fast::encode(ss, b1);

        auto b2 = std::uint8_t{0};
        xson::fast::decode(ss, b2);

        require_eq(b1, b2);
    };

    test_case("UInteger32") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::uint32_t{1234567};
        xson::fast::encode(ss, i1);

        auto i2 = std::uint32_t{0};
        xson::fast::decode(ss, i2);

        require_eq(i1, i2);
    };

    test_case("Integer32") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::int32_t{-1234567};
        xson::fast::encode(ss, i1);

        auto i2 = std::int32_t{0};
        xson::fast::decode(ss, i2);

        require_eq(i1, i2);
    };

    test_case("UInteger64") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::uint64_t{1234567};
        xson::fast::encode(ss, i1);

        auto i2 = std::uint64_t{0};
        xson::fast::decode(ss, i2);

        require_eq(i1, i2);
    };

    test_case("Integer64") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::int64_t{-1234567898765432};
        xson::fast::encode(ss, i1);

        auto i2 = std::int64_t{1};
        xson::fast::decode(ss, i2);
        require_eq(i1, i2);
    };

    test_case("String") = [] {
        auto ss = std::stringstream{};

        const auto s1 = "1234567 Kaius Ruokonen \n\t x"s;
        xson::fast::encode(ss, s1);

        auto s2 = ""s;
        xson::fast::decode(ss, s2);

        require_eq(s1, s2);
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::fast_test

