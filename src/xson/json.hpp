#pragma once

#include "xson/object.hpp"
#include "xson/json/encoder.hpp"
#include "xson/json/decoder.hpp"

namespace xson::json {

using object = xson::object;

inline object parse(std::istream& is)
{
    object ob;
    decoder{is}.decode(ob);
    return std::move(ob);
}

inline std::string stringify(const object& ob)
{
    std::stringstream ss;
    encoder{ss}.encode(ob);
    return ss.str();
}

} // namespace xson::json
