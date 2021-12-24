# json4cpp
JSON Library for C++20

## JSON Object ##
```cpp
#include "xson/json.hpp"

using namespace xson;

[...]

auto document = object{};

document["papa"s]["name"s] = "Cool"s;
document["papa"s]["age"s] = 40;
document["papa"s]["married"s] = false;
document["papa"s]["kids"s][1] = {"Name"s,"Tulppu"s};
document["papa"s]["kids"s][2] = {"Name"s,"Elppu"s};
document["papa"s]["kids"s][3] = {"Name"s,"Jalppu"s};

clog << json::stringify(document) << endl;
```

## JSON Parser ##
```cpp
#include "xson/json.hpp"

using namespace std;
using namespace string_literals;
using namespace xson;

[...]

auto ss = stringstream{R"(
    {
        "_id" : 2,
        "Name" : "Ruoka",
        "Embedded" : {
            "_id" : 5,
            "Name" : "Tuma"
        },
        "Lucky Numbers" : [
            2,
            22,
            2112
        ]
    }
)"};

clog << ss.str() << "\n\n";

auto result = json::parse(ss);

clog << setw(2) << result << "\n\n";

clog << "_id            = " << result["_id"s]               << "\n"
     << "Name           = " << result["Name"s]              << "\n"
     << "Embeded.Name   = " << result["Embedded"s]["Name"s] << "\n"
     << "Lucky Number 1 = " << result["Lucky Numbers"s][0]  << "\n"
     << "Lucky Number 2 = " << result["Lucky Numbers"s][1]  << "\n"
     << "Lucky Number 3 = " << result["Lucky Numbers"s][2]  << "\n\n";

int id = result["_id"s];

string name = result["Name"s];

int number = result["Lucky Numbers"s][1];
```
