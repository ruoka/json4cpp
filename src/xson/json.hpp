#pragma once

#include "xson/object.hpp"
#include "xson/json/encoder.hpp"
#include "xson/json/decoder.hpp"

namespace xson {
namespace json {

using object = xson::object;

inline object parse(std::istream& is)
{
    object ob;
    encoder{is}.encode(ob);
    return std::move(ob);
}

inline std::string stringify(const object& ob)
{
    std::stringstream ss;
    decoder{ss}.decode(ob);
    return ss.str();
}

} // namespace json
} // namespace xson
