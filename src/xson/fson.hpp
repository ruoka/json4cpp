#pragma once

#include "xson/fson/decoder.hpp"
#include "xson/fson/encoder.hpp"
#include "xson/builder.hpp"

namespace xson::fson {

using object = xson::object;

inline object parse(std::istream& is)
{
    auto b = xson::builder{};
    auto d = decoder<xson::builder>{b};
    d.decode(is);
    return b.get();
}

#ifndef XSON_FSON_HIDE_IOSTREAM

inline std::istream& operator >> (std::istream& is, object& ob)
{
    ob = xson::fson::parse(is);
    return is;
}

inline std::ostream& operator << (std::ostream& os, const object& ob)
{
    xson::fson::encoder{}.encode(os,ob);
    return os;
}

#endif

} // namespace xson::fson
