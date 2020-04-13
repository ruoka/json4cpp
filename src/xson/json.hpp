#pragma once

#include "xson/object.hpp"
#include "xson/json/encoder.hpp"
#include "xson/json/decoder.hpp"

namespace xson::json {

using object = xson::object;

// inline object parse(std::istream& is)
// {
//     auto ob = object{};
//     xson::json::_1::decoder{is}.decode(ob);
//     return std::move(ob);
// }

inline object parse(std::istream& is)
{
    auto b = xson::json::_2::builder{};
    auto p = xson::json::_2::parser{&b};
    p.parse(is);
    return b.get();
}

inline object parse(std::string_view sv)
{
    auto b = xson::json::_2::builder{};
    auto p = xson::json::_2::parser{&b};
    p.parse(sv);
    return b.get();
}

inline std::string stringify(const object& ob, unsigned indent = 2)
{
    auto ss = std::stringstream{};
    encoder{ss,indent}.encode(ob);
    return ss.str();
}

} // namespace xson::json
