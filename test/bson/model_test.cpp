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

TEST_F(BsonModelTest,Double)
{
    bson::document d
    {
      {"Double"s, 12.55}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}

TEST_F(BsonModelTest,String)
{
    bson::document d
    {
      {"String"s, "B"s}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}


TEST_F(BsonModelTest,Array)
{
    bson::document d
    {
      {"Array"s, bson::array{"a"s, "b"s, "c"s, "d"s}}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}

TEST_F(BsonModelTest,BooleanTrue)
{
    bson::document d
    {
      {"Boolean"s, false}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}

TEST_F(BsonModelTest,BooleanFalse)
{
    bson::document d
    {
      {"Boolean"s, true}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}

TEST_F(BsonModelTest,Integer32)
{
    bson::document d
    {
      {"Integer"s, 9}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}

TEST_F(BsonModelTest,Long64)
{
    bson::document d
    {
      {"Long"s, 2112ll}
    };
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
}

TEST_F(BsonModelTest,Mix)
{
    bson::document d
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
    ofs.write(d.cbegin(), d.size());
    ofs << endl;
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

    ofs.write(papa.data(), papa.size());
    ofs << endl;
}
