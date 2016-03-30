#include <sstream>
#include <limits>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/fson.hpp"

using namespace std::string_literals;
using namespace xson::fson;

TEST(XsonFsonTest, Array)
{
    xson::fson::object o1
    {
        { "Test"s,  std::vector<std::string>{"A","B","C"}}
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s][0].value(), o2["Test"s][0].value());
    EXPECT_EQ(o1["Test"s][1].value(), o2["Test"s][1].value());
    EXPECT_EQ(o1["Test"s][2].value(), o2["Test"s][2].value());
}

TEST(XsonFsonTest, Object)
{
    xson::fson::object o1
    {
        { "Test"s,  xson::fson::object{ {"A", 1}, {"B", 2.0}, {"C", false}}}
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s]["A"s].value(), o2["Test"s]["A"s].value());
    EXPECT_EQ(o1["Test"s]["B"s].value(), o2["Test"s]["B"s].value());
    EXPECT_EQ(o1["Test"s]["C"s].value(), o2["Test"s]["C"s].value());
}

TEST(XsonFsonTest, Int32)
{
    xson::fson::object o1
    {
        { "Test"s, 57}
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Int64)
{
    xson::fson::object o1
    {
        { "Test"s,  57ll}
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Double)
{
    xson::fson::object o1
    {
        { "Test"s, 57.99999 }
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}
TEST(XsonFsonTest, String)
{
    xson::fson::object o1
    {
        { "Test"s,  "Tulppu"s }
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Boolean)
{
    xson::fson::object o1
    {
        { "Test"s, true}
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Date)
{
    xson::fson::object o1
    {
        { "Test"s, std::chrono::system_clock::now()},
        { "A"s, true }
    };

    std::string expected = o1["Test"s];
    const std::chrono::system_clock::time_point us1 = o1["Test"s];
    ASSERT_EQ(xson::type::date, o1["Test"s].type());

    std::stringstream ss;
    ss << o1;
    std::clog << xson::json::stringify(o1) << std::endl;

    xson::fson::object o2;
    ss >> o2;
    std::clog << xson::json::stringify(o2) << std::endl;

    std::string actual = o2["Test"s];
    const std::chrono::system_clock::time_point us2 = o2["Test"s];
    ASSERT_EQ(xson::type::date, o2["Test"s].type());

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
    EXPECT_EQ(expected, actual);
}

TEST(XsonFsonTest, Null)
{
    xson::fson::object o1
    {
        { "Test"s,  nullptr}
    };

    std::stringstream ss;
    ss << o1;

    xson::fson::object o2;
    ss >> o2;

    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Int32MinMax)
{
    std::stringstream ss;
    ss << xson::fson::object{
        {"Zero", std::int32_t{0}},
        {"Min", std::numeric_limits<std::int32_t>::min()},
        {"Max", std::numeric_limits<std::int32_t>::max()}
    };

    auto ob = xson::fson::parse(ss);

    EXPECT_EQ(xson::type::int32, ob["Zero"s].type());
    int zero = ob["Zero"s];
    EXPECT_EQ(0, zero);

    EXPECT_EQ(xson::type::int32, ob["Min"s].type());
    int min = ob["Min"s];
    EXPECT_EQ(std::numeric_limits<int>::min(), min);

    EXPECT_EQ(xson::type::int32, ob["Max"s].type());
    int max = ob["Max"s];
    EXPECT_EQ(std::numeric_limits<int>::max(), max);
}

TEST(XsonFsonTest, Int64MinMax1)
{
    xson::fson::object ob =
    {
        {"Zero", std::int64_t{0}},
        {"Min", std::numeric_limits<std::int64_t>::min()},
        {"Max", std::numeric_limits<std::int64_t>::max()}
    };

    EXPECT_EQ(xson::type::int64, ob["Zero"s].type());
    long long zero = ob["Zero"s];
    EXPECT_EQ(0, zero);

    EXPECT_EQ(xson::type::int64, ob["Min"s].type());
    long long min = ob["Min"s];
    EXPECT_EQ(std::numeric_limits<long long>::min(), min);

    EXPECT_EQ(xson::type::int64, ob["Max"s].type());
    long long max = ob["Max"s];
    EXPECT_EQ(std::numeric_limits<long long>::max(), max);
}

TEST(XsonFsonTest, Int64MinMax2)
{
    std::stringstream ss;
    ss << xson::fson::object{
        {"Zero", std::int64_t{0}},
        {"Min", -36028797018963968ll}, // -2^56
        {"Max", 36028797018963968ll}   //  2^56
    };

    auto ob = xson::fson::parse(ss);

    EXPECT_EQ(xson::type::int64, ob["Zero"s].type());
    long long zero = ob["Zero"s];
    EXPECT_EQ(0, zero);

    EXPECT_EQ(xson::type::int64, ob["Min"s].type());
    long long min = ob["Min"s];
    EXPECT_EQ(-36028797018963968ll, min);

    EXPECT_EQ(xson::type::int64, ob["Max"s].type());
    long long max = ob["Max"s];
    EXPECT_EQ(36028797018963968ll, max);
}
