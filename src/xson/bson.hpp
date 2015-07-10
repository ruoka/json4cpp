#pragma once

#include "xson/object.hpp"
#include "xson/bson/encoder.hpp"
#include "xson/bson/decoder.hpp"

namespace xson {
namespace bson {

using object = xson::object;

inline object parse(std::istream& is)
{
    object ob;
    encoder{is}.encode(ob);
    return std::move(ob);
}

inline std::string stringify(const object& ob)
{
    decoder dc{ob};
    return std::string{dc.data(), dc.size()};
}

} // namespace bson
} // namespace xson
