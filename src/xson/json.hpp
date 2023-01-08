#pragma once

#include "xson/json/decoder.hpp"
#include "xson/json/encoder.hpp"
#include "xson/builder.hpp"

namespace xson::json {

using object = xson::object;

inline object parse(std::istream& is)
{
    auto b = xson::builder{};
    auto d = decoder<xson::builder>{b};
    d.decode(is);
    return b.get();
}

inline object parse(std::string_view sv)
{
    auto b = xson::builder{};
    auto d = decoder<xson::builder>{b};
    d.decode(sv);
    return b.get();
}

inline std::string stringify(const object& ob, unsigned indent = 2)
{
    auto ss = std::stringstream{};
    encoder{indent}.encode(ss,ob);
    return ss.str();
}

#ifndef XSON_JSON_HIDE_IOSTREAM

inline auto& operator >> (std::istream& is, object& ob)
{
    ob = xson::json::parse(is);
    return is;
}

inline auto& operator << (std::ostream& os, const object& obj)
{
    const auto indent = os.width();
    auto e = xson::json::encoder{indent};
    os.width(0);
    e.encode(os,obj);
    os.width(indent);
    return os;
}

#endif

} // namespace xson::json
