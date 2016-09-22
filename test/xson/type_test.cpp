#include <iostream>
#include <gtest/gtest.h>
#include "xson/object.hpp"

using namespace xson;

using string = std::string;
using nullptr_t = xson::null_type;
using date = std::chrono::system_clock::time_point;
using array1 = std::array<int,3>;
using vector1 = std::vector<string>;
using array2 = std::array<object,5>;
using vector2 = std::vector<object>;

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
    ASSERT_EQ(xson::is_value<array1>(),    false);
    ASSERT_EQ(xson::is_value<vector1>(),   false);
    ASSERT_EQ(xson::is_value<int[99]>(),   false);
    ASSERT_EQ(xson::is_value<array2>(),    false);
    ASSERT_EQ(xson::is_value<vector2>(),   false);
}

TEST(XsonObjectTest,IsValueArray)
{
    ASSERT_EQ(xson::is_value_array<int>(),       false);
    ASSERT_EQ(xson::is_value_array<long long>(), false);
    ASSERT_EQ(xson::is_value_array<double>(),    false);
    ASSERT_EQ(xson::is_value_array<bool>(),      false);
    ASSERT_EQ(xson::is_value_array<string>(),    false);
    ASSERT_EQ(xson::is_value_array<date>(),      false);
    ASSERT_EQ(xson::is_value_array<nullptr_t>(), false);
    ASSERT_EQ(xson::is_value_array<object>(),    false);
    ASSERT_EQ(xson::is_value_array<array1>(),    true);
    ASSERT_EQ(xson::is_value_array<vector1>(),   true);
    ASSERT_EQ(xson::is_value_array<int[99]>(),   true);
    ASSERT_EQ(xson::is_value_array<array2>(),    false);
    ASSERT_EQ(xson::is_value_array<vector2>(),   false);
}

TEST(XsonObjectTest,IsObjectArray)
{
    ASSERT_EQ(xson::is_object_array<int>(),       false);
    ASSERT_EQ(xson::is_object_array<long long>(), false);
    ASSERT_EQ(xson::is_object_array<double>(),    false);
    ASSERT_EQ(xson::is_object_array<bool>(),      false);
    ASSERT_EQ(xson::is_object_array<string>(),    false);
    ASSERT_EQ(xson::is_object_array<date>(),      false);
    ASSERT_EQ(xson::is_object_array<nullptr_t>(), false);
    ASSERT_EQ(xson::is_object_array<object>(),    false);
    ASSERT_EQ(xson::is_object_array<array1>(),    false);
    ASSERT_EQ(xson::is_object_array<vector1>(),   false);
    ASSERT_EQ(xson::is_object_array<int[99]>(),   false);
    ASSERT_EQ(xson::is_object_array<array2>(),    true);
    ASSERT_EQ(xson::is_object_array<vector2>(),   true);
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
    ASSERT_EQ(xson::to_type(xson::int64_type{}), type::int64);
}
