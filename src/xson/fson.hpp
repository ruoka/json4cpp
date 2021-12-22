#pragma once

#include "xson/object.hpp"
#include "xson/fson/encoder.hpp"
#include "xson/fson/decoder.hpp"

namespace xson::fson {

using object = xson::object;

inline object parse(std::istream& is)
{
    auto ob = object{};
    decoder{is}.decode(ob);
    return ob;
}

} // namespace xson::fson
