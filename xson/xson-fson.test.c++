// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::string_literals;
using namespace std::chrono;
using namespace xson;
using namespace xson::fson;

using xson::json::operator <<;
using xson::json::operator >>;

namespace xson::fson_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("Array, [xson]") = [] {
        auto o1 = object{"Test"s, {"A"s, "B"s, "C"s}};
        succeed(xson::json::stringify(o1, 2));

        check_true(o1["Test"s].is_array());
        check_true(o1["Test"s][0].is_string());
        check_true(o1["Test"s][1].is_string());
        check_true(o1["Test"s][2].is_string());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);
        succeed(xson::json::stringify(o2, 2));

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s][0], o2["Test"s][0]);
        check_eq(o1["Test"s][1], o2["Test"s][1]);
        check_eq(o1["Test"s][2], o2["Test"s][2]);
    };

    test_case("Object, [xson]") = [] {
        auto o1 = object{"Test"s, {{"A"s, 1}, {"B"s, 2.0}, {"C"s, false}}};
        succeed(xson::json::stringify(o1, 2));

        check_true(o1["Test"s].is_object());
        check_true(o1["Test"s]["A"s].is_integer());
        check_true(o1["Test"s]["B"s].is_number());
        check_true(o1["Test"s]["C"s].is_boolean());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);
        succeed(xson::json::stringify(o2, 2));

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s]["A"s], o2["Test"s]["A"s]);
        check_eq(o1["Test"s]["B"s], o2["Test"s]["B"s]);
        check_eq(o1["Test"s]["C"s], o2["Test"s]["C"s]);
        check_eq(o1["Test"s]["A"s].get<object::value>(), o2["Test"s]["A"s].get<object::value>());
        check_eq(o1["Test"s]["B"s].get<object::value>(), o2["Test"s]["B"s].get<object::value>());
        check_eq(o1["Test"s]["C"s].get<object::value>(), o2["Test"s]["C"s].get<object::value>());
    };

    test_case("Int32, [xson]") = [] {
        auto o1 = object{"Test"s, 57};

        succeed(xson::json::stringify(o1, 2));

        check_true(o1["Test"s].is_integer());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);

        succeed(xson::json::stringify(o2, 2));

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
    };

    test_case("Int64, [xson]") = [] {
        auto o1 = object{"Test"s,  57ll};

        check_true(o1["Test"s].is_integer());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
    };

    test_case("Double, [xson]") = [] {
        auto o1 = object{"Test"s, 57.99999 };

        check_true(o1["Test"s].is_number());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
    };

    test_case("String, [xson]") = [] {
        auto o1 = object{"Test"s, "Tulppu"s};

        succeed(xson::json::stringify(o1, 2));

        check_true(o1["Test"s].is_string());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);

        succeed(xson::json::stringify(o2, 2));

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
    };

    test_case("Boolean, [xson]") = [] {
        auto o1 = object{"Test"s, true};

        succeed(xson::json::stringify(o1, 2));

        check_true(o1["Test"s].is_boolean());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);

        succeed(xson::json::stringify(o2, 2));

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
    };

    test_case("Date, [xson]") = [] {
        auto o1 = object
        {
            {"Test"s, floor<milliseconds>(system_clock::now()) },
            {"A"s,    true                }
        };

        require_true(o1["Test"s].is_timestamp());
        require_true(o1["A"s].is_boolean());

        const std::chrono::system_clock::time_point expected = o1["Test"s];

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        std::clog << xson::json::stringify(o1, 2) << std::endl;

        auto o2 = object{};
        o2 = xson::fson::parse(ss);
        std::clog << xson::json::stringify(o2, 2) << std::endl;

        require_true(o2["Test"s].is_timestamp());
        require_true(o2["A"s].is_boolean());

        const system_clock::time_point actual = o2["Test"s];

        check_eq(expected, actual);
        check_eq(xson::to_string(expected), xson::to_string(actual));
    };

    test_case("Null, [xson]") = [] {
        auto o1 = object{"Test"s, nullptr};

        check_true(o1["Test"s].is_null());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = object{};
        o2 = xson::fson::parse(ss);

        check_eq(o1["Test"s], o2["Test"s]);
        check_eq(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
    };

    test_case("Int32MinMax, [xson]") = [] {
        auto o1 = object
        {
            { "Zero"s, std::int32_t{0}                          },
            { "Min"s,  std::numeric_limits<std::int32_t>::min() },
            { "Max"s,  std::numeric_limits<std::int32_t>::max() }
        };

        check_true(o1["Zero"s].is_integer());
        check_true(o1["Min"s].is_integer());
        check_true(o1["Max"s].is_integer());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = xson::fson::parse(ss);

        check_eq(o1["Zero"s], o2["Zero"s]);
        check_eq(o1["Min"s], o2["Min"s]);
        check_eq(o1["Max"s], o2["Max"s]);

        const int zero = o2["Zero"s];
        check_eq(0, zero);
        const int min = o2["Min"s];
        check_eq(std::numeric_limits<int>::min(), min);
        const int max = o2["Max"s];
        check_eq(std::numeric_limits<int>::max(), max);
    };

    test_case("Int64MinMax1, [xson]") = [] {
        auto o1 = object
        {
            { "Zero"s, std::int64_t{0}},
            { "Min"s,  std::numeric_limits<std::int64_t>::min()},
            { "Max"s,  std::numeric_limits<std::int64_t>::max()}
        };

        check_true(o1["Zero"s].is_integer());
        check_true(o1["Min"s].is_integer());
        check_true(o1["Max"s].is_integer());

        std::int64_t zero = o1["Zero"s];
        check_eq(0, zero);
        std::int64_t min = o1["Min"s];
        check_eq(std::numeric_limits<std::int64_t>::min(), min);
        std::int64_t max = o1["Max"s];
        check_eq(std::numeric_limits<std::int64_t>::max(), max);
    };

    test_case("Int64MinMax2, [xson]") = [] {
        auto o1 = object
        {
            { "Zero"s, 0ll                  },
            { "Min"s,  -36028797018963968ll }, // -2^56
            { "Max"s,  36028797018963968ll  }  //  2^56
        };

        check_true(o1["Zero"s].is_integer());
        check_true(o1["Min"s].is_integer());
        check_true(o1["Max"s].is_integer());

        auto ss = std::stringstream{};
        xson::fson::encoder{}.encode(ss, o1);
        auto o2 = xson::fson::parse(ss);

        check_eq(o1["Zero"s], o2["Zero"s]);
        check_eq(o1["Min"s], o2["Min"s]);
        check_eq(o1["Max"s], o2["Max"s]);

        std::int64_t zero = o2["Zero"s];
        check_eq(0, zero);
        std::int64_t min = o2["Min"s];
        check_eq(-36028797018963968ll, min);
        std::int64_t max = o2["Max"s];
        check_eq(36028797018963968ll, max);
    };

    test_case("RootPrimitiveRoundTrip, [xson]") = [] {
        // FSON supports non-container top-level values as well.
        auto round_trip = [](const xson::object& in) {
            auto ss = std::stringstream{};
            xson::fson::encoder{}.encode(ss, in);
            return xson::fson::parse(ss);
        };

        {
            auto v = xson::object{};
            v = nullptr;
            const auto out = round_trip(v);
            require_true(out.is_null());
        }
        {
            auto v = xson::object{};
            v = true;
            const auto out = round_trip(v);
            require_true(out.is_boolean());
            require_true(static_cast<boolean_type>(out));
        }
        {
            auto v = xson::object{};
            v = std::int64_t{42};
            const auto out = round_trip(v);
            require_true(out.is_integer());
            require_eq(42, static_cast<integer_type>(out));
        }
        {
            auto v = xson::object{};
            v = 3.5;
            const auto out = round_trip(v);
            require_true(out.is_number());
            require_eq(3.5, static_cast<number_type>(out));
        }
        {
            auto v = xson::object{};
            v = "hi"s;
            const auto out = round_trip(v);
            require_true(out.is_string());
            require_eq("hi"s, static_cast<string_type>(out));
        }
    };

    test_case("Malformed_NameInsideArray_Throws, [xson]") = [] {
        // type::name is invalid inside arrays; decoder should reject it (not crash).
        auto ss = std::stringstream{};
        xson::fast::encode(ss, xson::fson::type::array);
        xson::fast::encode(ss, xson::fson::type::name);
        xson::fast::encode(ss, "bad"s);
        xson::fast::encode(ss, xson::fson::type::end);

        require_throws([&]{ auto ob = xson::fson::parse(ss); });
    };

    test_case("Malformed_TruncatedObject_Throws, [xson]") = [] {
        // Missing type::end should be reported as truncated input.
        auto ss = std::stringstream{};
        xson::fast::encode(ss, xson::fson::type::object);
        xson::fast::encode(ss, xson::fson::type::name);
        xson::fast::encode(ss, "a"s);
        xson::fast::encode(ss, xson::fson::type::integer);
        xson::fast::encode(ss, std::int64_t{1});
        // no end marker

        require_throws([&]{ auto ob = xson::fson::parse(ss); });
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::fson_test

