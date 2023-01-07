#pragma once

#include <cassert>
#include <type_traits>
#include "xson/object.hpp"
#include "xson/fast/encoder.hpp"
#include "xson/fson/types.hpp"

namespace xson::fson {

class encoder : public fast::encoder
{
public:

    encoder(std::ostream& os) : fast::encoder{os}
    {}

    using fast::encoder::encode;

    void encode(const xson::object& o)
    {
        auto type = make_type(o);

        encode(type);

        switch(type)
        {
            case type::object:
            for(const auto& [name,value] : o.get<object::map>())
            {
                encode(type::name); // type
                encode(name);       // name
                encode(value);      // object
            }
            encode(type::end);
            break;

            case type::array:
            for(const auto& value : o.get<object::array>())
            {
                encode(value);       // object
            }
            encode(type::end);
            break;

            case type::integer:
            encode(std::get<xson::integer_type>(o.get<object::value>()));
            break;

            case type::number:
            encode(std::get<xson::number_type>(o.get<object::value>()));
            break;

            case type::string:
            encode(std::get<xson::string_type>(o.get<object::value>()));
            break;

            case type::boolean:
            encode(std::get<xson::boolean_type>(o.get<object::value>()));
            break;

            case type::timestamp:
            encode(std::get<xson::timestamp_type>(o.get<object::value>()));
            break;

            case type::null:
            break;

            default:
            assert(false);
            break;
        }
    }

private:

    constexpr fson::type make_type(const object& o) const
    {
        if(o.is_object()) return fson::type::object;

        if(o.is_array()) return fson::type::array;

        if(o.is_number()) return fson::type::number;

        if(o.is_string()) return fson::type::string;

        if(o.is_boolean()) return fson::type::boolean;

        if(o.is_integer()) return fson::type::integer;

        if(o.is_timestamp()) return fson::type::timestamp;

        return fson::type::null;
    }
};

} // namespace xson::fson
