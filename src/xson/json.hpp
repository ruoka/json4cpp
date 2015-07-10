#pragma once

#include "xson/object.hpp"
#include "xson/json/encoder.hpp"
#include "xson/json/decoder.hpp"

namespace xson {
namespace json {

using object = xson::object;

inline object parse(std::istream& is)
{
    object obj;
    encoder::encode(is, obj);
    return std::move(obj);
}

inline std::string stringify(const object& obj)
{
    std::stringstream os;
    decoder::decode(os, obj);
    return os.str();
}

} // namespace json
} // namespace xson
