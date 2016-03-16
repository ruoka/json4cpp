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
    decoder{is}.decode(ob);
    return std::move(ob);
}

inline std::string stringify(const object& ob)
{
    encoder e{ob};
    return std::string{e.data(), e.size()};
}

} // namespace bson
} // namespace xson
