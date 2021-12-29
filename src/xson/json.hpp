#pragma once

#include "xson/json/decoder.hpp"
#include "xson/json/encoder.hpp"

namespace xson::json {

using object = xson::object;

inline object parse(std::istream& is)
{
    auto b = xson::builder{};
    auto d = xson::json::decoder{&b};
    d.decode(is);
    return b.get();
}

inline object parse(std::string_view sv)
{
    auto b = xson::builder{};
    auto d = xson::json::decoder{&b};
    d.decode(sv);
    return b.get();
}

inline std::string stringify(const object& ob, unsigned indent = 2)
{
    auto ss = std::stringstream{};
    encoder{ss,indent}.encode(ob);
    return ss.str();
}

} // namespace xson::json

#ifndef XSON_JSON_HIDE_IOSTREAM

namespace std {

inline auto& operator >> (std::istream& is, xson::object& ob)
{
    ob = xson::json::parse(is);
    return is;
}

inline auto& operator << (std::ostream& os, const xson::object& obj)
{
    const auto indent = os.width();
    auto e = xson::json::encoder{os,indent};
    os.width(0);
    e.encode(obj);
    os.width(indent);
    return os;
}

} // namespace std

#endif
