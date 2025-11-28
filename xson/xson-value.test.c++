// SPDX-License-Identifier: MIT
// See the LICENSE file in the project root for full license text.

import std;
import xson;
import tester;

using namespace std::string_literals;
using namespace xson;
using namespace xson::json;

namespace xson::value_test {

auto register_tests()
{
    using tester::basic::test_case;
    using namespace tester::assertions;

    test_case("CompareInt32") = [] {
        object::value val1{1};
        object::value val2{1};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareInt64") = [] {
        object::value val1{1ll};
        object::value val2{1ll};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareString") = [] {
        object::value val1{"A"s};
        object::value val2{"A"s};
        require_true(val1 == val2);
        require_false(val1 != val2);
    };

    test_case("CompareBoolean") = [] {
        object::value val1{true};
        object::value val2{false};
        require_true(val1 != val2);
        require_false(val1 == val2);
    };

    test_case("ToStringInt32") = [] {
        object::value val1{123};
        object::value val2{456};
        require_eq("123", xson::to_string(val1));
        require_eq("456", xson::to_string(val2));
    };

    test_case("ToStringBoolean") = [] {
        object::value val1{true};
        object::value val2{false};
        require_eq("true", xson::to_string(val1));
        require_eq("false", xson::to_string(val2));
    };

    return 0;
}

const auto _ = register_tests();

} // namespace xson::value_test

