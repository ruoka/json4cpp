#include <iostream>
#include <gtest/gtest.h>
#include "xson/json.hpp"

using namespace xson;
using namespace json;

TEST(XsonValueTest,CompareInt32)
{
    xson::value val1{1};
    xson::value val2{1};
    ASSERT_TRUE(val1 == val2);
    ASSERT_FALSE(val1 != val2);
}

TEST(XsonValueTest,CompareInt64)
{
    xson::value val1{1ll};
    xson::value val2{1ll};
    ASSERT_TRUE(val1 == val2);
    ASSERT_FALSE(val1 != val2);
}

TEST(XsonValueTest,CompareString)
{
    xson::value val1{"A"s};
    xson::value val2{"A"s};
    ASSERT_TRUE(val1 == val2);
    ASSERT_FALSE(val1 != val2);
}

TEST(XsonValueTest,CompareBoolean)
{
    xson::value val1{true};
    xson::value val2{false};
    ASSERT_TRUE(val1 != val2);
    ASSERT_FALSE(val1 == val2);
}

TEST(XsonValueTest,ToStringInt32)
{
    xson::value val1{123};
    xson::value val2{456};
    ASSERT_EQ("123", xson::to_string(val1));
    ASSERT_EQ("456", xson::to_string(val2));
}

TEST(XsonValueTest,ToStringBoolean)
{
    xson::value val1{true};
    xson::value val2{false};
    ASSERT_EQ("true", xson::to_string(val1));
    ASSERT_EQ("false", xson::to_string(val2));
}
