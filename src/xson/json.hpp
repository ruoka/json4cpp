#pragma once

#include "xson/object.hpp"
#include "xson/json/encoder.hpp"
#include "xson/json/decoder.hpp"

namespace xson::json {

using object = xson::object;

inline object parse(std::istream& is)
{
    auto ob = object{};
    decoder{is}.decode(ob);
    return std::move(ob);
}

inline std::string stringify(const object& ob, unsigned indent = 2)
{
    auto ss = std::stringstream{};
    encoder{ss,indent}.encode(ob);
    return ss.str();
}

} // namespace xson::json
