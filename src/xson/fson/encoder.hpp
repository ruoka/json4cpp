#pragma once

#include "xson/fast/encoder.hpp"
#include "xson/object.hpp"

namespace xson {
namespace fson {

class encoder : public fast::encoder
{
public:

    encoder(std::ostream& os) : fast::encoder(os)
    {}

    using fast::encoder::encode;

    void encode(const object&)
    {
        // FIXME
    }

};

} // namespace fson
} // namespace xson
