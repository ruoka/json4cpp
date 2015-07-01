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
        ofs.open("test1.bson");
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

    ofs << doc << endl;
}
*/

TEST_F(BsonModelTest,Double)
{
    bson::document doc
    {
      {"Double"s, 12.55}
    };

    ofs << doc << endl;
}

TEST_F(BsonModelTest,String)
{
    bson::document doc
    {
      {"String"s, "B"s}
    };

    ofs << doc << endl;
}


TEST_F(BsonModelTest,Array)
{
    bson::document doc
    {
      {"Array"s, bson::array{"a"s, "b"s, "c"s, "d"s}}
    };

    ofs << doc << endl;
}

TEST_F(BsonModelTest,BooleanTrue)
{
    bson::document doc
    {
      {"Boolean"s, false}
    };

    ofs << doc << endl;
}

TEST_F(BsonModelTest,BooleanFalse)
{
    bson::document doc
    {
      {"Boolean"s, true}
    };

    ofs << doc << endl;
}

TEST_F(BsonModelTest,Integer32)
{
    bson::document doc
    {
      {"Integer"s, 9}
    };

    ofs << doc << endl;
}

TEST_F(BsonModelTest,Long64)
{
    bson::document doc
    {
      {"Long"s, 2112ll}
    };

    ofs << doc << endl;
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

    ofs << doc << endl;
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

    bson::document papa
    {
      {"Name","Papa Cool"s},
      {"Age",39},
      {"Sons",sons},
      {"Sizes",sizes},
      {"LuckyNumbers", bson::array{2,22,2112}}
    };

    ofs << papa << endl;
}
