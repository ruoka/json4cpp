#include <sstream>
#include <limits>
#include <gtest/gtest.h>
#define XSON_JSON_HIDE_IOSTREAM
#include "xson/fson.hpp"
#include "xson/json.hpp"

using namespace std;
using namespace string_literals;
using namespace chrono;
using namespace xson;
using namespace xson::fson;

TEST(XsonFsonTest, Array)
{
    auto o1 = object{"Test"s, {"A"s, "B"s, "C"s}};
    TRACE(json::stringify(o1));

    EXPECT_TRUE(o1["Test"s].is_array());
    EXPECT_TRUE(o1["Test"s][0].is_string());
    EXPECT_TRUE(o1["Test"s][1].is_string());
    EXPECT_TRUE(o1["Test"s][2].is_string());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;
    TRACE(json::stringify(o2));

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s][0], o2["Test"s][0]);
    EXPECT_EQ(o1["Test"s][1], o2["Test"s][1]);
    EXPECT_EQ(o1["Test"s][2], o2["Test"s][2]);
}

TEST(XsonFsonTest, Object)
{
    auto o1 = object{"Test"s, {{"A"s, 1}, {"B"s, 2.0}, {"C"s, false}}};
    TRACE(json::stringify(o1));

    EXPECT_TRUE(o1["Test"s].is_object());
    EXPECT_TRUE(o1["Test"s]["A"s].is_integer());
    EXPECT_TRUE(o1["Test"s]["B"s].is_number());
    EXPECT_TRUE(o1["Test"s]["C"s].is_boolean());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;
    TRACE(json::stringify(o2));

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s]["A"s], o2["Test"s]["A"s]);
    EXPECT_EQ(o1["Test"s]["B"s], o2["Test"s]["B"s]);
    EXPECT_EQ(o1["Test"s]["C"s], o2["Test"s]["C"s]);
    EXPECT_EQ(o1["Test"s]["A"s].get<object::value>(), o2["Test"s]["A"s].get<object::value>());
    EXPECT_EQ(o1["Test"s]["B"s].get<object::value>(), o2["Test"s]["B"s].get<object::value>());
    EXPECT_EQ(o1["Test"s]["C"s].get<object::value>(), o2["Test"s]["C"s].get<object::value>());
}

TEST(XsonFsonTest, Int32)
{
    auto o1 = object{"Test"s, 57};

    TRACE(json::stringify(o1));

    EXPECT_TRUE(o1["Test"s].is_integer());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    TRACE(json::stringify(o2));

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
}

TEST(XsonFsonTest, Int64)
{
    auto o1 = object{"Test"s,  57ll};

    EXPECT_TRUE(o1["Test"s].is_integer());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
}

TEST(XsonFsonTest, Double)
{
    auto o1 = object{"Test"s, 57.99999 };

    EXPECT_TRUE(o1["Test"s].is_number());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
}

TEST(XsonFsonTest, String)
{
    auto o1 = object{"Test"s, "Tulppu"s};

    TRACE(json::stringify(o1));

    EXPECT_TRUE(o1["Test"s].is_string());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    TRACE(json::stringify(o2));

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
}

TEST(XsonFsonTest, Boolean)
{
    auto o1 = object{"Test"s, true};

    TRACE(json::stringify(o1));

    EXPECT_TRUE(o1["Test"s].is_boolean());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    TRACE(json::stringify(o2));

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
}

TEST(XsonFsonTest, Date)
{
    auto o1 = object
    {
        {"Test"s, floor<milliseconds>(system_clock::now()) },
        {"A"s,    true                }
    };

    ASSERT_TRUE(o1["Test"s].is_timestamp());
    ASSERT_TRUE(o1["A"s].is_boolean());

    const chrono::system_clock::time_point expected = o1["Test"s];

    auto ss = stringstream{};
    ss << o1;
    clog << json::stringify(o1) << endl;

    auto o2 = object{};
    ss >> o2;
    clog << json::stringify(o2) << endl;

    ASSERT_TRUE(o2["Test"s].is_timestamp());
    ASSERT_TRUE(o2["A"s].is_boolean());

    const system_clock::time_point actual = o2["Test"s];

    EXPECT_EQ(expected, actual);
    EXPECT_EQ(xson::to_string(expected), xson::to_string(actual));
}

TEST(XsonFsonTest, Null)
{
    auto o1 = object{"Test"s, nullptr};

    EXPECT_TRUE(o1["Test"s].is_null());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s], o2["Test"s]);
    EXPECT_EQ(o1["Test"s].get<object::value>(), o2["Test"s].get<object::value>());
}

TEST(XsonFsonTest, Int32MinMax)
{
    auto o1 = object
    {
        { "Zero"s, int32_t{0}                          },
        { "Min"s,  numeric_limits<int32_t>::min() },
        { "Max"s,  numeric_limits<int32_t>::max() }
    };

    EXPECT_TRUE(o1["Zero"s].is_integer());
    EXPECT_TRUE(o1["Min"s].is_integer());
    EXPECT_TRUE(o1["Max"s].is_integer());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = parse(ss);

    EXPECT_EQ(o1["Zero"s], o2["Zero"s]);
    EXPECT_EQ(o1["Min"s], o2["Min"s]);
    EXPECT_EQ(o1["Max"s], o2["Max"s]);

    const int zero = o2["Zero"s];
    EXPECT_EQ(0, zero);
    const int min = o2["Min"s];
    EXPECT_EQ(numeric_limits<int>::min(), min);
    const int max = o2["Max"s];
    EXPECT_EQ(numeric_limits<int>::max(), max);
}

TEST(XsonFsonTest, Int64MinMax1)
{
    auto o1 = object
    {
        { "Zero"s, int64_t{0}},
        { "Min"s,  numeric_limits<int64_t>::min()},
        { "Max"s,  numeric_limits<int64_t>::max()}
    };

    EXPECT_TRUE(o1["Zero"s].is_integer());
    EXPECT_TRUE(o1["Min"s].is_integer());
    EXPECT_TRUE(o1["Max"s].is_integer());

    std::int64_t zero = o1["Zero"s];
    EXPECT_EQ(0, zero);
    std::int64_t min = o1["Min"s];
    EXPECT_EQ(numeric_limits<std::int64_t>::min(), min);
    std::int64_t max = o1["Max"s];
    EXPECT_EQ(numeric_limits<std::int64_t>::max(), max);
}

TEST(XsonFsonTest, Int64MinMax2)
{
    auto o1 = object
    {
        { "Zero"s, 0ll                  },
        { "Min"s,  -36028797018963968ll }, // -2^56
        { "Max"s,  36028797018963968ll  }  //  2^56
    };

    EXPECT_TRUE(o1["Zero"s].is_integer());
    EXPECT_TRUE(o1["Min"s].is_integer());
    EXPECT_TRUE(o1["Max"s].is_integer());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = parse(ss);

    EXPECT_EQ(o1["Zero"s], o2["Zero"s]);
    EXPECT_EQ(o1["Min"s], o2["Min"s]);
    EXPECT_EQ(o1["Max"s], o2["Max"s]);

    std::int64_t zero = o2["Zero"s];
    EXPECT_EQ(0, zero);
    std::int64_t min = o2["Min"s];
    EXPECT_EQ(-36028797018963968ll, min);
    std::int64_t max = o2["Max"s];
    EXPECT_EQ(36028797018963968ll, max);
}
