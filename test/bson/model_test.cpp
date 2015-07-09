#include <fstream>
#include <gtest/gtest.h>
#include "bson/model.hpp"

using namespace std;
using namespace std::literals::string_literals;

class BsonModelTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        ofs.open("./test/test1.bson");
    }

    std::ofstream ofs;
};

/*
TEST_F(BsonModelTest,Unknown)
{
    bson::document doc
    {
      {"Double"s, "Static assert test"}
    };

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}
*/

TEST_F(BsonModelTest,Double)
{
    bson::document doc
    {
      {"Double"s, 12.55}
    };

    ASSERT_EQ(doc.size(), 4 + 6 + 1 + 8); // int32 + bytes*6 + \x00 + int64

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,String)
{
    bson::document doc
    {
      {"String"s, "B"s}
    };

    ASSERT_EQ(doc.size(), 4 + 6 + 1 + 4 + 1 + 1); // int32 + bytes*6 + \x00 + int32 + bytes*1 + \x00

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,Document)
{
    bson::document doc
    {
      {"A"s, 1},{"B"s,true},{"C"s, 21.12}
    };

    ASSERT_EQ(doc.size(), 6 + 4 + 6 + 1 + 6 + 8); // (int32 + bytes*2 + int32) etc.

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,Array)
{
    auto arr = bson::array{"a"s, "b"s, "c"s, "d"s};

    ASSERT_EQ(arr.size(), 4 * ( 4 + 1 + 1 + 4 +1 +1)); // 4 * (int32 + bytes*1 + \x00 + int32 + bytes*1  + \x00)

    bson::document doc
    {
      {"Array"s, arr}
    };

    ASSERT_EQ(doc.size(), 4 + 10 + arr.size() + 1);

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,BooleanTrue)
{
    bson::document doc
    {
      {"Boolean"s, false}
    };

    ASSERT_EQ(doc.size(), 4 + 7 + 1 + 1); // int32 + bytes*7 + \x00 + \x01

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,BooleanFalse)
{
    bson::document doc
    {
      {"Boolean"s, false}
    };

    ASSERT_EQ(doc.size(), 4 + 7 + 1 + 1); // int32 + bytes*7 + \x00 + \x00

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,Integer32)
{
    bson::document doc
    {
      {"Integer"s, 9}
    };

    ASSERT_EQ(doc.size(), 4 + 7 + 1 + 4); // int32 + bytes*7 + \x00 + int32

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,Long64)
{
    bson::document doc
    {
      {"Long"s, 2112ll}
    };

    ASSERT_EQ(doc.size(), 4 + 4 + 1 + 8); // int32 + bytes*4 + \x00 + int64

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,Mix)
{
    bson::document doc
    {
      {"Double"s, 12.55},
      {"String"s, "B"s},
      {"DOCUMENT"s, bson::document{{"a"s, "b"s}, {"c"s, 1ll}}},
      {"Array"s, bson::array{"a"s, "b"s, "c"s, "d"s}},
      {"Boolean"s, false},
      {"Boolean"s, true},
      {"Integer"s, 9},
      {"Long"s, 2112ll}
    };

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}

TEST_F(BsonModelTest,Nested)
{
    bson::document sons
    {
      {"Name","Tulppu"s},
      {"Name","Elppu"s},
      {"Name","Jalppu"s}
    };

    bson::array sizes
    {
      bson::document {"ShoeSize",47.50},
      bson::document {"WaistSize",120.50}
    };

    bson::document doc
    {
      {"Name","Papa Cool"s},
      {"Age",39},
      {"Sons",sons},
      {"Sizes",sizes},
      {"LuckyNumbers", bson::array{2,22,2112}}
    };

    auto p1 = ofs.tellp();
    ofs << doc << flush;
    auto p2 = ofs.tellp();

    ASSERT_EQ(p2 - p1, 4 + doc.size() + 1); // int32 + doc + \x00
}
