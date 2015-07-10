#pragma once

#include "xson/object.hpp"
#include "xson/bson/encoder.hpp"
#include "xson/bson/decoder.hpp"

namespace xson {
namespace bson {

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

} // namespace bson
} // namespace xson
