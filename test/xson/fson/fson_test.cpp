#include <sstream>
#include <limits>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/fson.hpp"

using namespace std;
using namespace string_literals;
using namespace chrono;
using namespace xson;
using namespace xson::fson;

TEST(XsonFsonTest, Array)
{
    auto o1 = object{u8"Test"s, {u8"A"s, u8"B"s, u8"C"s}};

    EXPECT_EQ(xson::type::array, o1["Test"s].type());
    EXPECT_EQ(xson::type::string, o1["Test"s][0].type());
    EXPECT_EQ(xson::type::string, o1["Test"s][1].type());
    EXPECT_EQ(xson::type::string, o1["Test"s][2].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s][0].type(), o2["Test"s][0].type());
    EXPECT_EQ(o1["Test"s][1].type(), o2["Test"s][1].type());
    EXPECT_EQ(o1["Test"s][2].type(), o2["Test"s][2].type());
    EXPECT_EQ(o1["Test"s][0].type(), o2["Test"s][0].type());
    EXPECT_EQ(o1["Test"s][1].type(), o2["Test"s][1].type());
    EXPECT_EQ(o1["Test"s][2].type(), o2["Test"s][2].type());
}

TEST(XsonFsonTest, Object)
{
    auto o1 = object{u8"Test"s, {{u8"A"s, 1}, {u8"B"s, 2.0}, {u8"C"s, false}}};

    clog << json::stringify(o1) << endl;

    EXPECT_EQ(xson::type::object, o1["Test"s].type());
    EXPECT_EQ(xson::type::int32, o1["Test"s]["A"s].type());
    EXPECT_EQ(xson::type::number, o1["Test"s]["B"s].type());
    EXPECT_EQ(xson::type::boolean, o1["Test"s]["C"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s]["A"s].type(), o2["Test"s]["A"s].type());
    EXPECT_EQ(o1["Test"s]["B"s].type(), o2["Test"s]["B"s].type());
    EXPECT_EQ(o1["Test"s]["C"s].type(), o2["Test"s]["C"s].type());
    EXPECT_EQ(o1["Test"s]["A"s].value(), o2["Test"s]["A"s].value());
    EXPECT_EQ(o1["Test"s]["B"s].value(), o2["Test"s]["B"s].value());
    EXPECT_EQ(o1["Test"s]["C"s].value(), o2["Test"s]["C"s].value());
}

TEST(XsonFsonTest, Int32)
{
    auto o1 = object{u8"Test"s, 57};

    EXPECT_EQ(xson::type::int32, o1["Test"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Int64)
{
    auto o1 = object{u8"Test"s,  57ll};

    EXPECT_EQ(xson::type::int64, o1["Test"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Double)
{
    auto o1 = object{u8"Test"s, 57.99999 };

    EXPECT_EQ(xson::type::number, o1["Test"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, String)
{
    auto o1 = object{u8"Test"s, u8"Tulppu"s};

    EXPECT_EQ(xson::type::string, o1["Test"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Boolean)
{
    auto o1 = object{u8"Test"s, true};

    EXPECT_EQ(xson::type::boolean, o1["Test"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Date)
{
    auto o1 = object
    {
        {u8"Test"s, system_clock::now() },
        {u8"A"s,    true                }
    };

    ASSERT_EQ(xson::type::date, o1["Test"s].type());
    ASSERT_EQ(xson::type::boolean, o1["A"s].type());

    const string expected = o1["Test"s];
    const chrono::system_clock::time_point us1 = o1["Test"s];

    auto ss = stringstream{};
    ss << o1;
    clog << json::stringify(o1) << endl;

    auto o2 = object{};
    ss >> o2;
    clog << json::stringify(o2) << endl;

    ASSERT_EQ(xson::type::date, o2["Test"s].type());
    ASSERT_EQ(xson::type::boolean, o2["A"s].type());

    const string actual = o2["Test"s];
    const system_clock::time_point us2 = o2["Test"s];

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
    EXPECT_EQ(expected, actual);
}

TEST(XsonFsonTest, Null)
{
    auto o1 = object{u8"Test"s, nullptr};

    EXPECT_EQ(xson::type::null, o1["Test"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = object{};
    ss >> o2;

    EXPECT_EQ(o1["Test"s].type(), o2["Test"s].type());
    EXPECT_EQ(o1["Test"s].value(), o2["Test"s].value());
}

TEST(XsonFsonTest, Int32MinMax)
{
    auto o1 = object
    {
        { u8"Zero"s, int32_t{0}                          },
        { u8"Min"s,  numeric_limits<int32_t>::min() },
        { u8"Max"s,  numeric_limits<int32_t>::max() }
    };

    EXPECT_EQ(xson::type::int32, o1["Zero"s].type());
    EXPECT_EQ(xson::type::int32, o1["Min"s].type());
    EXPECT_EQ(xson::type::int32, o1["Max"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = parse(ss);

    EXPECT_EQ(o1["Zero"s].type(), o2["Zero"s].type());
    EXPECT_EQ(o1["Min"s].type(), o2["Min"s].type());
    EXPECT_EQ(o1["Max"s].type(), o2["Max"s].type());

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
        { u8"Zero"s, int64_t{0}},
        { u8"Min"s,  numeric_limits<int64_t>::min()},
        { u8"Max"s,  numeric_limits<int64_t>::max()}
    };

    EXPECT_EQ(xson::type::int64, o1["Zero"s].type());
    EXPECT_EQ(xson::type::int64, o1["Min"s].type());
    EXPECT_EQ(xson::type::int64, o1["Max"s].type());

    long long zero = o1["Zero"s];
    EXPECT_EQ(0, zero);
    long long min = o1["Min"s];
    EXPECT_EQ(numeric_limits<long long>::min(), min);
    long long max = o1["Max"s];
    EXPECT_EQ(numeric_limits<long long>::max(), max);
}

TEST(XsonFsonTest, Int64MinMax2)
{
    auto o1 = object
    {
        { u8"Zero"s, 0ll                  },
        { u8"Min"s,  -36028797018963968ll }, // -2^56
        { u8"Max"s,  36028797018963968ll  }  //  2^56
    };

    EXPECT_EQ(xson::type::int64, o1["Zero"s].type());
    EXPECT_EQ(xson::type::int64, o1["Min"s].type());
    EXPECT_EQ(xson::type::int64, o1["Max"s].type());

    auto ss = stringstream{};
    ss << o1;
    auto o2 = parse(ss);

    EXPECT_EQ(o1["Zero"s].type(), o2["Zero"s].type());
    EXPECT_EQ(o1["Min"s].type(), o2["Min"s].type());
    EXPECT_EQ(o1["Max"s].type(), o2["Max"s].type());

    long long zero = o2["Zero"s];
    EXPECT_EQ(0, zero);
    long long min = o2["Min"s];
    EXPECT_EQ(-36028797018963968ll, min);
    long long max = o2["Max"s];
    EXPECT_EQ(36028797018963968ll, max);
}
