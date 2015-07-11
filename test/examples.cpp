#include <vector>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/bson.hpp"

using namespace std;
using namespace xson;
using namespace literals::string_literals;

TEST(Examples,Stringify)
{
    xson::object sons
    {
        { "Name", "Tulppu"s },
        { "Name", "Elppu"s  },
        { "Name", "Jalppu"s }
    };

    std::vector<object> sizes
    {
        xson::object{ "ShoeSize",  47.50  },
        xson::object{ "WaistSize", 120.50 }
    };

    xson::object papa
    {
        { "Name",         "Papa Cool"s                  },
        { "Age",          39                            },
        { "Sons",         sons                          },
        { "Sizes",        sizes                         },
        { "LuckyNumbers", std::vector<int>{2, 22, 2112} }
    };

    clog << json::stringify(papa) << endl;

    clog << bson::stringify(papa) << endl;
}

TEST(Examples,Parse)
{
    using namespace xson::json;

    stringstream ss;
    ss << "{\"_id\":2,\"Name\":\"Ruoka\", \"Embedded\":{\"_id\":5,\"Name\":\"Tuma\"}, \"Lucky Numbers\":[2,22,2112]}"s;

    clog << ss.str() << endl;

    auto result = json::parse(ss);

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
}
