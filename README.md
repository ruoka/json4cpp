# json4cpp
Jason Library for C++14

## Model ##
```cpp
    #include "json/model.hpp"

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

    std::clog << papa << std::endl;

    std::clog << json::document{{"_id", 2,},{"Name","Dodo"s}} << std::endl;
```
    
## Parser ##
```cpp
    #include "json/parser.hpp"

    std::stringstream ios;
    ios << "{\"_id\":2,\"Name\":\"Ruoka\", \"Embedded\":{\"_id\":5,\"Name\":\"Tuma\"}, \"Lucky Numbers\":[2,22,2112] }" << std::endl;

    std::clog << ios.str() << std::endl;

    auto result = json::parse(ios);

    std::clog << result << std::endl;

    std::clog << "_id = "  << result["_id"s]  << "\n"
              << "name = " << result["Name"s] << "\n"
              << "name = " << result["Embedded"s]["Name"s]  << "\n"
              << "number 1 = " << result["Lucky Numbers"s][0]  << "\n"
              << "number 2 = " << result["Lucky Numbers"s]["2"s]  << "\n" << std::endl;

    int id = result["_id"s];
    std::string name = result["Name"s];
    double number = result["Lucky Numbers"s][1];              
```
