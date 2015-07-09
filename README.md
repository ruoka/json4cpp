# json4cpp
JSON Library for C++14

## Model ##
```cpp
#include "json/model.hpp"
using namespace std;
using namespace literals::string_literals;

[...]

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
```

## Parser ##
```cpp
#include "json/parser.hpp"
using namespace std;
using namespace literals::string_literals;

[...]

stringstream ios;
ios << "{\"_id\":2,\"Name\":\"Ruoka\", \"Embedded\":{\"_id\":5,\"Name\":\"Tuma\"}, \"Lucky Numbers\":[2,22,2112] }" << endl;

clog << ios.str() << endl;

auto result = json::parse(ios);

clog << result << endl;

clog << "_id = "  << result["_id"s]                           << "\n"
     << "Name = " << result["Name"s]                          << "\n"
     << "Embeded.Name = "   << result["Embedded"s]["Name"s]   << "\n"
     << "Lucky Number 1 = " << result["Lucky Numbers"s][0]    << "\n"
     << "Lucky Number 2 = " << result["Lucky Numbers"s][1]    << "\n"
     << "Lucky Number 3 = " << result["Lucky Numbers"s]["2"s] << endl;

int id = result["_id"s];
string name = result["Name"s];
double number = result["Lucky Numbers"s][1];
```
