#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include "json/model.hpp"

using namespace std;
using namespace std::literals::string_literals;

TEST(JsonModelTest,Mockup)
{
    json::document sons
    {
        {"Name","Tulppu"s},
        {"Name","Elppu"s},
        {"Name","Jalppu"s}
    };

    json::array sizes
    {
        json::document {"ShoeSize",47.50},
        json::document {"WaistSize",120.50}
    };

    json::document papa
    {
        {"Name","Papa Cool"s},
        {"Age",39u},
        {"Sons",sons},
        {"Sizes",sizes},
        {"LuckyNumbers", json::array{2u,22u,2112u}}
    };

    clog << papa << endl;

    clog << json::document{{"_id", 2,},{"Name","Dodo"s}} << endl;
}
