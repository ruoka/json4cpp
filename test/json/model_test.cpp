#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include "json/model.hpp"

using namespace std::literals::string_literals;
using std::clog;
using std::endl;
using json::document;
using json::array;

TEST(JsonModelTest,ComplexDocument)
{
    document sons
    {
        {"Name","Tulppu"s},
        {"Name","Elppu"s},
        {"Name","Jalppu"s}
    };

    array sizes
    {
        document{"ShoeSize",47.50},
        document{"WaistSize",120.50}
    };

    document papa
    {
        {"Name","Papa Cool"s},
        {"Age",39u},
        {"Sons",sons},
        {"Sizes",sizes},
        {"LuckyNumbers", array{2u,22u,2112u}}
    };

    clog << papa << endl;

    clog << document{{"_id",2,},{"Name","Dodo"s}} << endl;
}
