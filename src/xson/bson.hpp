#pragma once

#include "xson/object.hpp"
#include "xson/bson/encoder.hpp"
#include "xson/bson/decoder.hpp"

namespace xson::bson {

using object = xson::object;

inline object parse(std::istream& is)
{
    auto ob = object{};
    decoder{is}.decode(ob);
    return std::move(ob);
}

inline std::string stringify(const object& ob)
{
    const auto e = encoder{ob};
    return std::string{e.data(), e.size()};
}

} // namespace xson::bson
