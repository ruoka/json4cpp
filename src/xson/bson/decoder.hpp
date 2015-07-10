#pragma once

#include "xson/object.hpp"

namespace xson {
namespace bson {

std::ostream& operator << (std::ostream& os, const object& obj);

class decoder
{
public:

    static void decode(std::ostream& os, const object& obj)
    {
        //decode_document(os, obj);
    }
};

inline std::ostream& operator << (std::ostream& os, const object& obj)
{
    decoder::decode(os, obj);
    return os;
}

} // namespace bson
} // namespace xson
