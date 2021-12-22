#include <vector>
#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"

using namespace std;
using namespace string_literals;
using namespace xson;

TEST(Examples,Stringify)
{
    auto kids = vector<object>
    {
        { "Name"s, "Tulppu"s },
        { "Name"s, "Elppu"s  },
        { "Name"s, "Jalppu"s }
    };

    auto sizes = object
    {
        { "Height"s,   200   },
        { "Waist"s,    120.5 },
        { "ShoeSize"s, 47.5  }
    };

    auto papa = object
    {
        { "Name",         "Papa"s       },
        { "Age",          40            },
        { "Kids",         kids          },
        { "Measures",     sizes         },
        { "LuckyNumbers", {2, 22, 2112} },
        { "Lucky",        false         }
    };

    clog << json::stringify(papa) << endl;
}

TEST(Examples,Parse)
{
    using namespace json;

    auto ss = stringstream{R"(
        {
            "id" : 2,
            "Name" : "Ruoka",
            "Embedded" : {
                "id" : 5,
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

    clog << "_id            = " << result["id"s]                << "\n"
         << "Name           = " << result["Name"s]              << "\n"
         << "Embeded.Name   = " << result["Embedded"s]["Name"s] << "\n"
         << "Lucky Number 1 = " << result["Lucky Numbers"s][0]  << "\n"
         << "Lucky Number 2 = " << result["Lucky Numbers"s][1]  << "\n"
         << "Lucky Number 3 = " << result["Lucky Numbers"s][2]  << "\n\n";

    integer_type id = result["id"s];
    clog << id << "\n";

    string_type name = result["Name"s];
    clog << name << "\n";

    integer_type number = result["Lucky Numbers"s][1];
    clog << number << "\n";
}
