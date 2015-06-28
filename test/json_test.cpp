#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include "json.hpp"

using namespace std::literals::string_literals;

TEST(JSON,MockupTest)
{
    json::double_type b1;
    json::string_type b2;
    json::document_type b3;
    json::array_type b4;
    json::binary_type b5;
    struct foo {} b6;
    json::objectid_type b7;
    json::int32_type b16;
    json::int64_type b18;

    std::clog << json::type(b1) << "\n"
              << json::type(b2) << "\n"
              << json::type(b3) << "\n"
              << json::type(b4) << "\n"
              << json::type(b5) << "\n"
              << json::type(b6) << "\n"
              << json::type(b7) << "\n"
              << json::type(false) << "\n"
              << json::type(true) << "\n"
              << json::type(nullptr) << "\n"
              << json::type(b16) << "\n"
              << json::type(b18) << "\n";

    json::document sons
    {
        {"Name","Tulus"s},
        {"Name","Elo"s},
        {"Name","Jalo"s}
    };

    json::array sizes
    {
        json::document {"ShoeSize",47.5},
        json::document {"WaistSize",120}
    };

    json::document papa
    {
        {"Name","Kaius Ruokonen"s},
        {"Age",39u},
        {"Sons",sons},
        {"Sizes",sizes},
        {"LucyNumbers", json::array{2u,22u,2112u}}
    };

    std::clog << papa << std::endl;

    std::clog << json::document{{"_id", 2,},{"name","ruoka"}} << std::endl;

    std::clog << std::endl;
}
