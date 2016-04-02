#include <vector>
#include <fstream>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/bson.hpp"

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

    auto measures = object
    {
        { "Height"s,   200   },
        { "Waist"s,    120.5 },
        { "ShoeSize"s, 47.5  }
    };

    auto papa = object
    {
        { "Name",         "Cool Papa"s             },
        { "Age",          39                       },
        { "Kids",         kids                     },
        { "Measures",     measures                 },
        { "LuckyNumbers", vector<int>{2, 22, 2112} },
        { "Lucky",        false                    }
    };

    clog << json::stringify(papa) << endl;

    clog << bson::stringify(papa) << endl;
}

TEST(Examples,Parse)
{
    using namespace json;
    auto ss = stringstream{"{\"_id\":2,\"Name\":\"Ruoka\", \"Embedded\":{\"_id\":5,\"Name\":\"Tuma\"}, \"Lucky Numbers\":[2,22,2112]}"s};

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

    double number = result["Lucky Numbers"s][1];
}

TEST(Examples,BsonDump)
{
    ifstream fs{"./test/xson/test3.bson"};
    while(fs && !fs.eof())
    {
        clog << json::stringify(bson::parse(fs), 4) << endl;
        fs >> ws;
    }
}
