#pragma once

#include "xson/object.hpp"
#include "xson/fson/encoder.hpp"
#include "xson/fson/decoder.hpp"

namespace xson::fson {

using object = xson::object;

inline object parse(std::istream& is)
{
    object ob;
    decoder{is}.decode(ob);
    return std::move(ob);
}

} // namespace xson::fson
