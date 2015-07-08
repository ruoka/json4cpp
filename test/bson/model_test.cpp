#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include "bson/model.hpp"

using namespace std;
using namespace std::literals::string_literals;

TEST(BsonModelTest,Mockup)
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

    clog << papa << endl;

    clog << bson::document{{"_id", 2,},{"Name","Ruoka"s}} << endl;
}
