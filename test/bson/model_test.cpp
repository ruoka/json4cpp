#include <iostream>
#include <gtest/gtest.h>
#include "bson/model.hpp"

using namespace std;
using namespace std::literals::string_literals;

TEST(BsonModelTest,Double)
{
    bson::document d
    {
      {"Double"s, 12.55}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,String)
{
    bson::document d
    {
      {"String"s, "B"s}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}


TEST(BsonModelTest,Array)
{
    bson::document d
    {
      {"Array"s, bson::array{"a"s, "b"s, "c"s, "d"s}}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,BooleanTrue)
{
    bson::document d
    {
      {"Boolean"s, false}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,BooleanFalse)
{
    bson::document d
    {
      {"Boolean"s, true}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,Integer32)
{
    bson::document d
    {
      {"Integer"s, 9}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,Long64)
{
    bson::document d
    {
      {"Long"s, 2112ll}
    };
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,Mix)
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
    clog << d.size() << ": ";
    clog.write(d.cbegin(), d.size());
    clog << endl;
}

TEST(BsonModelTest,Nested)
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

    clog << papa.size() << ": ";
    clog.write(papa.cbegin(), papa.size());
    clog << endl;
}
