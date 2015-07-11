#include <fstream>
#include <gtest/gtest.h>
#include "xson/bson.hpp"

using namespace std;
using namespace std::literals::string_literals;
using namespace xson;
using namespace xson::bson;

class XsonBsonDecoderTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        ofs.open("./test/xson/bson/test1.bson");
    }

    std::ofstream ofs;
};

TEST_F(XsonBsonDecoderTest,Double)
{
    bson::object bob
    {
      {"Double"s, 12.55}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 4 + 6 + 1 + 8; // int32 + bytes*6 + \x00 + int64
    size += 4 + 1;              // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

TEST_F(XsonBsonDecoderTest,String)
{
    bson::object bob
    {
      {"String"s, "B"s}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 4 + 6 + 1 + 4 + 1 + 1; // int32 + bytes*6 + \x00 + int32 + bytes*1 + \x00
    size += 4 + 1;                     // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

TEST_F(XsonBsonDecoderTest,bobument)
{
    bson::object bob
    {
      {"A"s, 1}, {"B"s,true}, {"C"s, 21.12}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 6 + 4 + 6 + 1 + 6 + 8; // (int32 + bytes*2 + int32) etc.
    size += 4 + 1;                     // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

// TEST_F(XsonBsonDecoderTest,Array)
// {
//     auto arr = bson::array{"a"s, "b"s, "c"s, "d"s};
//
//     ASSERT_EQ(arr.size(), 4 * ( 4 + 1 + 1 + 4 +1 +1)); // 4 * (int32 + bytes*1 + \x00 + int32 + bytes*1  + \x00)
//
//     bson::object bob
//     {
//       {"Array"s, arr}
//     };
//
//     ASSERT_EQ(bob.size(), 4 + 10 + arr.size() + 1);
//
//     auto p1 = ofs.tellp();
//     ofs << bob << flush;
//     auto p2 = ofs.tellp();
//
//     ASSERT_EQ(p2 - p1, 4 + bob.size() + 1); // int32 + bob + \x00
// }

TEST_F(XsonBsonDecoderTest,BooleanTrue)
{
    bson::object bob
    {
      {"Boolean"s, false}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 4 + 7 + 1 + 1; // int32 + bytes*7 + \x00 + \x01
    size += 4 + 1;             // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

TEST_F(XsonBsonDecoderTest,BooleanFalse)
{
    bson::object bob
    {
      {"Boolean"s, false}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 4 + 7 + 1 + 1; // int32 + bytes*7 + \x00 + \x01
    size += 4 + 1;             // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

TEST_F(XsonBsonDecoderTest,Integer32)
{
    bson::object bob
    {
      {"Integer"s, 9}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 4 + 7 + 1 + 4; // int32 + bytes*7 + \x00 + int32
    size += 4 + 1;             // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

TEST_F(XsonBsonDecoderTest,Long64)
{
    bson::object bob
    {
      {"Long"s, 2112ll}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    auto size = 4 + 4 + 1 + 8; // int32 + bytes*4 + \x00 + int64
    size += 4 + 1;             // int32 + bob + \x00
    ASSERT_EQ(size, p2 - p1);
}

TEST_F(XsonBsonDecoderTest,Mix)
{
    bson::object bob =
    {
      {"Double"s, 12.55},
      {"String"s, "B"s},
      {"bobUMENT"s, bson::object{{"a"s, "b"s}, {"c"s, 1ll}}},
      {"Array"s, std::array<std::string,4>{"a"s, "b"s, "c"s, "d"s}},
      {"Boolean"s, false},
      {"Boolean"s, true},
      {"Integer"s, 9},
      {"Long"s, 2112ll}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

//    ASSERT_EQ(p2 - p1, 4 + bob.size() + 1); // int32 + bob + \x00
}

TEST_F(XsonBsonDecoderTest,Nested)
{
    auto sons = bson::object
    {
        {"Name","Tulppu"s},
        {"Name","Elppu"s},
        {"Name","Jalppu"s}
    };

    auto sizes = std::array<bson::object,2>
    {
        bson::object {"ShoeSize",47.50},
        bson::object {"WaistSize",120.50}
    };

    auto bob = bson::object
    {
        {"Name","Papa Cool"s},
        {"Age",39},
        {"Sons",sons},
        {"Sizes",sizes},
        {"LuckyNumbers", std::array<int,3>{2,22,2112}}
    };

    auto p1 = ofs.tellp();
    ofs << bob << flush;
    auto p2 = ofs.tellp();

    //    ASSERT_EQ(p2 - p1, 4 + bob.size() + 1); // int32 + bob + \x00
}
