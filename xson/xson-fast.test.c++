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

    test_case("Byte, [xson]") = [] {
        auto ss = std::stringstream{};

        auto b1 = std::uint8_t{1};
        xson::fast::encode(ss, b1);

        auto b2 = std::uint8_t{0};
        xson::fast::decode(ss, b2);

        require_eq(b1, b2);
    };

    test_case("UInteger32, [xson]") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::uint32_t{1234567};
        xson::fast::encode(ss, i1);

        auto i2 = std::uint32_t{0};
        xson::fast::decode(ss, i2);

        require_eq(i1, i2);
    };

    test_case("Integer32, [xson]") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::int32_t{-1234567};
        xson::fast::encode(ss, i1);

        auto i2 = std::int32_t{0};
        xson::fast::decode(ss, i2);

        require_eq(i1, i2);
    };

    test_case("UInteger64, [xson]") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::uint64_t{1234567};
        xson::fast::encode(ss, i1);

        auto i2 = std::uint64_t{0};
        xson::fast::decode(ss, i2);

        require_eq(i1, i2);
    };

    test_case("Integer64, [xson]") = [] {
        auto ss = std::stringstream{};

        auto i1 = std::int64_t{-1234567898765432};
        xson::fast::encode(ss, i1);

        auto i2 = std::int64_t{1};
        xson::fast::decode(ss, i2);
        require_eq(i1, i2);
    };

    test_case("Integer64 full range round-trip, [xson]") = [] {
        auto round_trip = [](std::int64_t in) {
            auto ss = std::stringstream{};
            xson::fast::encode(ss, in);
            auto out = std::int64_t{0};
            xson::fast::decode(ss, out);
            return out;
        };

        require_eq(round_trip(std::numeric_limits<std::int64_t>::min()),
                   std::numeric_limits<std::int64_t>::min());
        require_eq(round_trip(std::numeric_limits<std::int64_t>::max()),
                   std::numeric_limits<std::int64_t>::max());
        require_eq(round_trip(0x4000000000000000ll), 0x4000000000000000ll); // 2^62
        require_eq(round_trip(-0x4000000000000000ll - 1), -0x4000000000000000ll - 1);
    };

    test_case("UInteger64 full range round-trip, [xson]") = [] {
        auto round_trip = [](std::uint64_t in) {
            auto ss = std::stringstream{};
            xson::fast::encode(ss, in);
            auto out = std::uint64_t{0};
            xson::fast::decode(ss, out);
            return out;
        };

        require_eq(round_trip(0ull), 0ull);
        require_eq(round_trip(0x7fffffffffffffffull), 0x7fffffffffffffffull); // 2^63-1
        require_eq(round_trip(0x8000000000000000ull), 0x8000000000000000ull); // 2^63
        require_eq(round_trip(std::numeric_limits<std::uint64_t>::max()),
                   std::numeric_limits<std::uint64_t>::max());
    };

    test_case("String, [xson]") = [] {
        auto ss = std::stringstream{};

        const auto s1 = "1234567 Kaius Ruokonen \n\t x"s;
        xson::fast::encode(ss, s1);

        auto s2 = ""s;
        xson::fast::decode(ss, s2);

        require_eq(s1, s2);
    };

    test_case("String UTF-8 round-trip, [xson]") = [] {
        // Single-pass encode: ASCII bypasses the escape branch; high bytes are
        // streamed as ESC+hex with no temporary and no pre-scan.
        auto round_trip = [](const std::string& in) {
            auto ss = std::stringstream{};
            xson::fast::encode(ss, in);
            auto out = ""s;
            xson::fast::decode(ss, out);
            return out;
        };

        require_eq(round_trip("plain"s), "plain"s);
        require_eq(round_trip("café 世界 🌍"s), "café 世界 🌍"s);
        // Split the literal so \x01 is not greedily parsed as \x01b.
        require_eq(round_trip("a\x01" "b"s), "a\x01" "b"s);
    };

    test_case("String malformed escape throws, [xson]") = [] {
        require_throws([] {
            auto s = "\x01"s;
            unescape_inplace(s);
        });
        require_throws([] {
            auto s = "\x01G0"s;
            unescape_inplace(s);
        });
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::fast_test

