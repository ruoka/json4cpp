#include <iostream>
#include <gtest/gtest.h>
#include "xson/object.hpp"

using namespace xson;

using string = std::string;
using nullptr_t = std::nullptr_t;
using date = std::chrono::system_clock::time_point;
using array = std::array<int,9>;
using vector = std::vector<string>;

TEST(XsonTypeTest,IsValue)
{
    ASSERT_EQ(xson::is_value<int>(),       true);
    ASSERT_EQ(xson::is_value<long long>(), true);
    ASSERT_EQ(xson::is_value<double>(),    true);
    ASSERT_EQ(xson::is_value<bool>(),      true);
    ASSERT_EQ(xson::is_value<string>(),    true);
    ASSERT_EQ(xson::is_value<date>(),      true);
    ASSERT_EQ(xson::is_value<nullptr_t>(), true);
    ASSERT_EQ(xson::is_value<object>(),    false);
    ASSERT_EQ(xson::is_value<array>(),     false);
    ASSERT_EQ(xson::is_value<vector>(),    false);
    ASSERT_EQ(xson::is_value<int[99]>(),   false);
}

TEST(XsonObjectTest,IsArray)
{
    ASSERT_EQ(xson::is_array<int>(),       false);
    ASSERT_EQ(xson::is_array<long long>(), false);
    ASSERT_EQ(xson::is_array<double>(),    false);
    ASSERT_EQ(xson::is_array<bool>(),      false);
    ASSERT_EQ(xson::is_array<string>(),    false);
    ASSERT_EQ(xson::is_array<date>(),      false);
    ASSERT_EQ(xson::is_array<nullptr_t>(), false);
    ASSERT_EQ(xson::is_array<object>(),    false);
    ASSERT_EQ(xson::is_array<array>(),     true);
    ASSERT_EQ(xson::is_array<vector>(),    true);
    ASSERT_EQ(xson::is_array<int[99]>(),   true);
}

TEST(XsonObjectTest,ToType)
{
    ASSERT_EQ(xson::to_type(double{}),       type::number);
    ASSERT_EQ(xson::to_type(string{}),       type::string);
//  ASSERT_EQ(xson::to_type(object{}),       type::object);
//  ASSERT_EQ(xson::to_type(array{}),        type::array);
    ASSERT_EQ(xson::to_type(bool{}),         type::boolean);
    ASSERT_EQ(xson::to_type(date{}),         type::date);
    ASSERT_EQ(xson::to_type(nullptr),        type::null);
    ASSERT_EQ(xson::to_type(int{}),          type::int32);
    ASSERT_EQ(xson::to_type(std::int64_t{}), type::int64);
}
