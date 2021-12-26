#pragma once

#include "xson/fson/decoder.hpp"
#include "xson/fson/encoder.hpp"

namespace xson::fson {

inline object parse(std::istream& is)
{
    auto b = xson::builder{};
    auto d = xson::fson::decoder{is,&b};
    d.decode();
    return b.get();
}


#ifndef XSON_FSON_HIDE_IOSTREAM

inline auto& operator >> (std::istream& is, xson::object& ob)
{
    ob = xson::fson::parse(is);
    return is;
}

inline auto& operator << (std::ostream& os, const xson::object& ob)
{
    xson::fson::encoder{os}.encode(ob);
    return os;
}

#endif

} // namespace xson::fson
