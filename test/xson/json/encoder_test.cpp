#include <vector>
#include <gtest/gtest.h>
#include "xson/json/encoder.hpp"

using namespace std;
using namespace std::chrono;
using namespace xson;
using namespace xson::json;

TEST(XsonJsonEncoderTest, Ostream)
{
    auto mix = xson::object
    {
        { "Ruoka",  true                     },
        { "Onni",   false                    },
        { "Tulppu", 1.0                      },
        { "Elppu",  2.0                      },
        { "Jalppu", 3.0                      },
        { "Ages",   vector<int>{39,40,9,5,2} },
        { "Now",    system_clock::now()      }
    };

    clog << setw(0) << mix << endl;

    clog << setw(3) << mix << endl;

    clog << setw(6) << mix << endl;

    clog << setw(12) << mix << endl;
}
