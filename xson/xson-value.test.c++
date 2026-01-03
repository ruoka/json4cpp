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
        object::primitive val1{1};
        object::primitive val2{1};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareInt64, [xson]") = [] {
        object::primitive val1{1ll};
        object::primitive val2{1ll};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareString, [xson]") = [] {
        object::primitive val1{"A"s};
        object::primitive val2{"A"s};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareBoolean, [xson]") = [] {
        object::primitive val1{true};
        object::primitive val2{false};
        require_true(val1 != val2);
        require_false(val1 == val2);
    };

    test_case("ToStringInt32, [xson]") = [] {
        object::primitive val1{123};
        object::primitive val2{456};
        require_eq("123", xson::to_string(val1));
        require_eq("456", xson::to_string(val2));
    };

    test_case("ToStringBoolean, [xson]") = [] {
        object::primitive val1{true};
        object::primitive val2{false};
        require_eq("true", xson::to_string(val1));
        require_eq("false", xson::to_string(val2));
    };

    test_case("ToStringInt64, [xson]") = [] {
        object::primitive val1{1234567890123ll};
        object::primitive val2{-9876543210987ll};
        require_eq("1234567890123", xson::to_string(val1));
        require_eq("-9876543210987", xson::to_string(val2));
    };

    test_case("ToStringDouble, [xson]") = [] {
        object::primitive val1{3.14159};
        object::primitive val2{-42.5};
        object::primitive val3{0.0};
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
        object::primitive val1{"hello"s};
        object::primitive val2{"world"s};
        object::primitive val3{""s};
        require_eq("hello", xson::to_string(val1));
        require_eq("world", xson::to_string(val2));
        require_eq("", xson::to_string(val3));
    };

    test_case("ToStringNull, [xson]") = [] {
        object::primitive val1{std::monostate{}};
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
        
        object::primitive val1{tp};
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
        object::primitive val{tp};
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
        
        // Parse back using to_time_point (if accessible)
        // Note: to_time_point is not exported, so we can't test it directly
        // But we can verify the format is correct
        require_eq(24u, iso.size());
        require_eq('Z', iso.back());
        
        // Verify the ISO string can be used to reconstruct (if we had access to to_time_point)
        // For now, just verify format correctness
        require_true(iso[0] >= '0' && iso[0] <= '9');  // Year starts with digit
    };

    test_case("ToStringObjectValueAllTypes, [xson]") = [] {
        // Test all variant types in object::primitive
        object::primitive val_int32{42};
        object::primitive val_int64{1234567890123ll};
        object::primitive val_double{3.14};
        object::primitive val_string{"test"s};
        object::primitive val_bool{true};
        object::primitive val_null{std::monostate{}};
        auto tp = system_clock::now();
        object::primitive val_timestamp{tp};
        
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

