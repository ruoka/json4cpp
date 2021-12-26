#include <iostream>
#include <gtest/gtest.h>
#include "xson/json.hpp"

using namespace xson;
using namespace json;

TEST(XsonValueTest,CompareInt32)
{
    object::value val1{1};
    object::value val2{1};
    ASSERT_TRUE(val1 == val2);
    ASSERT_FALSE(val1 != val2);
}

TEST(XsonValueTest,CompareInt64)
{
    object::value val1{1ll};
    object::value val2{1ll};
    ASSERT_TRUE(val1 == val2);
    ASSERT_FALSE(val1 != val2);
}

TEST(XsonValueTest,CompareString)
{
    object::value val1{"A"s};
    object::value val2{"A"s};
    ASSERT_TRUE(val1 == val2);
    ASSERT_FALSE(val1 != val2);
}

TEST(XsonValueTest,CompareBoolean)
{
    object::value val1{true};
    object::value val2{false};
    ASSERT_TRUE(val1 != val2);
    ASSERT_FALSE(val1 == val2);
}

TEST(XsonValueTest,ToStringInt32)
{
    object::value val1{123};
    object::value val2{456};
    ASSERT_EQ("123", xson::to_string(val1));
    ASSERT_EQ("456", xson::to_string(val2));
}

TEST(XsonValueTest,ToStringBoolean)
{
    object::value val1{true};
    object::value val2{false};
    ASSERT_EQ("true", xson::to_string(val1));
    ASSERT_EQ("false", xson::to_string(val2));
}
