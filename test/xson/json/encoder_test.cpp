#include <vector>
#include <gtest/gtest.h>
#include "xson/json.hpp"
#include "xson/trace.hpp"

using namespace std;
using namespace string_literals;
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
    ASSERT_EQ(type::string, ob["First"s].type());
    ASSERT_EQ(type::string, ob["Second"s].type());
    ASSERT_EQ(type::string, ob["Third"s].type());
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
    ASSERT_EQ(type::array,  ob["Kids"s].type());
    ASSERT_EQ(type::object, ob["Kids"s][0].type());
    ASSERT_EQ(type::object, ob["Kids"s][1].type());
    ASSERT_EQ(type::object, ob["Kids"s][2].type());
    ASSERT_EQ(type::string, ob["Kids"s][0]["Name"s].type());
    ASSERT_EQ(type::string, ob["Kids"s][1]["Name"s].type());
    ASSERT_EQ(type::string, ob["Kids"s][2]["Name"s].type());
}
