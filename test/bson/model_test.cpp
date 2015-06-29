#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include "bson/model.hpp"

using namespace std;
using namespace std::literals::string_literals;

TEST(BsonModelTest,Mockup)
{
    bson::double_type b1;
    bson::string_type b2;
    bson::document_type b3;
    bson::array_type b4;
    bson::binary_type b5;
    struct foo {} b6;
    bson::objectid_type b7;
    bson::int32_type b16;
    bson::int64_type b18;

    std::clog << bson::type(b1) << "\n"
              << bson::type(b2) << "\n"
              << bson::type(b3) << "\n"
              << bson::type(b4) << "\n"
              << bson::type(b5) << "\n"
              << bson::type(b6) << "\n"
              << bson::type(b7) << "\n"
              << bson::type(false) << "\n"
              << bson::type(true) << "\n"
              << bson::type(nullptr) << "\n"
              << bson::type(b16) << "\n"
              << bson::type(b18) << "\n";

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
      {"Age",39u},
      {"Sons",sons},
      {"Sizes",sizes},
      {"LuckyNumbers", bson::array{2u,22u,2112u}}
    };

    clog << papa << endl;

    clog << bson::document{{"_id", 2,},{"Name","Ruoka"}} << endl;
}
