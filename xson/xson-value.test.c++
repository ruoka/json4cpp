// Copyright (c) 2025-2026 Kaius Ruokonen. All rights reserved.
// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::string_literals;
using namespace std::chrono;
using namespace xson;
using namespace xson::json;

namespace xson::value_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("CompareInt32, [xson]") = [] {
        primitive val1{1};
        primitive val2{1};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareInt64, [xson]") = [] {
        primitive val1{1ll};
        primitive val2{1ll};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareString, [xson]") = [] {
        primitive val1{"A"s};
        primitive val2{"A"s};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareBoolean, [xson]") = [] {
        primitive val1{true};
        primitive val2{false};
        require_true(val1 != val2);
        require_false(val1 == val2);
    };

    test_case("ToStringInt32, [xson]") = [] {
        primitive val1{123};
        primitive val2{456};
        require_eq("123", xson::to_string(val1));
        require_eq("456", xson::to_string(val2));
    };

    test_case("ToStringBoolean, [xson]") = [] {
        primitive val1{true};
        primitive val2{false};
        require_eq("true", xson::to_string(val1));
        require_eq("false", xson::to_string(val2));
    };

    test_case("ToStringInt64, [xson]") = [] {
        primitive val1{1234567890123ll};
        primitive val2{-9876543210987ll};
        require_eq("1234567890123", xson::to_string(val1));
        require_eq("-9876543210987", xson::to_string(val2));
    };

    test_case("ToStringDouble, [xson]") = [] {
        primitive val1{3.14159};
        primitive val2{-42.5};
        primitive val3{0.0};
        // Note: std::to_string for doubles may have precision differences
        // So we check that it contains the expected values
        auto s1 = xson::to_string(val1);
        auto s2 = xson::to_string(val2);
        auto s3 = xson::to_string(val3);
        require_true(s1.find("3.14") != std::string::npos || s1.find("3.141") != std::string::npos);
        require_true(s2.find("-42.5") != std::string::npos);
        require_eq("0.000000", s3);
    };

    test_case("ToStringString, [xson]") = [] {
        primitive val1{"hello"s};
        primitive val2{"world"s};
        primitive val3{""s};
        require_eq("hello", xson::to_string(val1));
        require_eq("world", xson::to_string(val2));
        require_eq("", xson::to_string(val3));
    };

    test_case("ToStringNull, [xson]") = [] {
        primitive val1{std::monostate{}};
        require_eq("null", xson::to_string(val1));
    };

    test_case("ToStringTimestamp, [xson]") = [] {
        // Create a specific timestamp for testing
        auto tp = system_clock::time_point{};
        tp += days{365 * 50};  // 50 years
        tp += hours{12};
        tp += minutes{30};
        tp += seconds{45};
        tp += milliseconds{123};
        
        primitive val1{tp};
        auto result = xson::to_string(val1);
        // Should be ISO8601 format: YYYY-MM-DDThh:mm:ss.fffZ
        require_true(result.size() >= 20);
        require_true(result.find('T') != std::string::npos);
        require_true(result.find('Z') != std::string::npos);
        require_true(result.find('-') != std::string::npos);
        require_true(result.find(':') != std::string::npos);
    };

    test_case("ToISO8601, [xson]") = [] {
        // Test with a known timestamp
        auto tp = system_clock::time_point{};
        tp += days{365 * 50 + 10};  // 50 years + 10 days
        tp += hours{14};
        tp += minutes{25};
        tp += seconds{30};
        tp += milliseconds{456};
        
        auto iso = xson::to_iso8601(tp);
        
        // Verify format: YYYY-MM-DDThh:mm:ss.fffZ
        require_eq(24u, iso.size());
        require_eq('T', iso[10]);
        require_eq('Z', iso[23]);
        require_eq('-', iso[4]);
        require_eq('-', iso[7]);
        require_eq(':', iso[13]);
        require_eq(':', iso[16]);
        require_eq('.', iso[19]);
        
        // Verify it matches to_string for timestamp
        primitive val{tp};
        require_eq(iso, xson::to_string(val));
    };

    test_case("ToISO8601VariousTimes, [xson]") = [] {
        // Test midnight
        auto tp1 = system_clock::time_point{};
        tp1 += days{365 * 50};
        auto iso1 = xson::to_iso8601(tp1);
        require_eq(24u, iso1.size());
        require_eq("00:00:00.000", iso1.substr(11, 12));
        
        // Test end of day
        auto tp2 = system_clock::time_point{};
        tp2 += days{365 * 50};
        tp2 += hours{23};
        tp2 += minutes{59};
        tp2 += seconds{59};
        tp2 += milliseconds{999};
        auto iso2 = xson::to_iso8601(tp2);
        require_eq(24u, iso2.size());
        require_eq("23:59:59.999", iso2.substr(11, 12));
        
        // Test noon
        auto tp3 = system_clock::time_point{};
        tp3 += days{365 * 50};
        tp3 += hours{12};
        auto iso3 = xson::to_iso8601(tp3);
        require_eq(24u, iso3.size());
        require_eq("12:00:00.000", iso3.substr(11, 12));
    };

    test_case("ToISO8601RoundTrip, [xson]") = [] {
        // Create a timestamp
        auto original = system_clock::now();
        original = time_point_cast<milliseconds>(original);  // Round to milliseconds
        
        // Convert to ISO8601
        auto iso = xson::to_iso8601(original);
        
        require_eq(24u, iso.size());
        require_eq('Z', iso.back());
        require_true(iso[0] >= '0' && iso[0] <= '9');  // Year starts with digit

        // Regression: to_time_point used unsequenced ++ptr / ptr+N in from_chars
        // arguments, which corrupted month/day/time on common compilers.
        const auto parsed = xson::to_time_point(iso);
        require_eq(duration_cast<milliseconds>(original.time_since_epoch()).count(),
                   duration_cast<milliseconds>(parsed.time_since_epoch()).count());
    };

    test_case("ToTimePointKnownValues, [xson]") = [] {
        // Epoch and pre-epoch must parse exactly (not a near-miss wrong date).
        const auto epoch = xson::to_time_point("1970-01-01T00:00:00.000Z");
        require_eq(0ll, duration_cast<milliseconds>(epoch.time_since_epoch()).count());

        const auto before = xson::to_time_point("1969-12-31T23:59:59.999Z");
        require_eq(-1ll, duration_cast<milliseconds>(before.time_since_epoch()).count());

        const auto noon = xson::to_time_point("2020-01-01T12:00:00.000Z");
        require_eq(1577880000000ll, duration_cast<milliseconds>(noon.time_since_epoch()).count());
        require_eq("2020-01-01T12:00:00.000Z", xson::to_iso8601(noon));

        // Date-only is midnight UTC.
        const auto date_only = xson::to_time_point("2020-01-01");
        require_eq(duration_cast<milliseconds>(
                       xson::to_time_point("2020-01-01T00:00:00.000Z").time_since_epoch()).count(),
                   duration_cast<milliseconds>(date_only.time_since_epoch()).count());
    };

    test_case("ToTimePointRejectsTruncatedOrCorrupt, [xson]") = [] {
        // Regression: lengths other than 10/24 used to parse the date only and
        // silently drop a present time (e.g. "…T12:00:00Z" → midnight).
        require_throws([&]{ (void)xson::to_time_point("2020-01-01T12:00:00Z"); });
        require_throws([&]{ (void)xson::to_time_point("2020-01-01T12:00:00.000"); });
        require_throws([&]{ (void)xson::to_time_point("2020-01-01T12:00:00.000z"); });

        // Failed/partial numeric fields used to leave zeros and build unspecified dates.
        require_throws([&]{ (void)xson::to_time_point("2020-1X-01T00:00:00.000Z"); });
        require_throws([&]{ (void)xson::to_time_point("2020-13-01T00:00:00.000Z"); });
        require_throws([&]{ (void)xson::to_time_point("2020-01-32T00:00:00.000Z"); });
        require_throws([&]{ (void)xson::to_time_point("2020-01-01T24:00:00.000Z"); });
        require_throws([&]{ (void)xson::to_time_point("1970/01/01T00:00:00.000Z"); });
    };

    test_case("ToStringObjectValueAllTypes, [xson]") = [] {
        // Test all variant types in primitive
        primitive val_int32{42};
        primitive val_int64{1234567890123ll};
        primitive val_double{3.14};
        primitive val_string{"test"s};
        primitive val_bool{true};
        primitive val_null{std::monostate{}};
        auto tp = system_clock::now();
        primitive val_timestamp{tp};
        
        require_eq("42", xson::to_string(val_int32));
        require_eq("1234567890123", xson::to_string(val_int64));
        require_true(xson::to_string(val_double).find("3.1") != std::string::npos);
        require_eq("test", xson::to_string(val_string));
        require_eq("true", xson::to_string(val_bool));
        require_eq("null", xson::to_string(val_null));
        
        auto ts_str = xson::to_string(val_timestamp);
        require_true(ts_str.size() >= 20);
        require_true(ts_str.find('T') != std::string::npos);
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::value_test

