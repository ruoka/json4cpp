#include <vector>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace std::string_literals;
using namespace xson;
using namespace json;

TEST(XsonJsonEncoderTest,Object)
{
    object ob
    {
        { "First",  "Tulppu"s },
        { "Second", "Elppu"s  },
        { "Third",  "Jalppu"s }
    };

    TRACE(ob);
}

TEST(XsonJsonEncoderTest,Array)
{
    object ob
    {
        "Kids",
        vector<object>
        {
            { "Name", "Tulppu"s },
            { "Name", "Elppu"s  },
            { "Name", "Jalppu"s }
        }
    };

    TRACE(ob);
}
