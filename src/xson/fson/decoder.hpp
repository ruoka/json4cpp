#pragma once

#include "xson/fast/decoder.hpp"
#include "xson/object.hpp"

namespace xson {
namespace fson {

class decoder : public fast::decoder
{
public:

    decoder(std::istream& is) : fast::decoder(is)
    {}

    using fast::decoder::decode;

    void decode(object&)
    {
        // FIXME
    }

};

} // namespace fson
} // namespace xson
