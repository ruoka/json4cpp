#pragma once

#include <cassert>
#include <type_traits>
#include "xson/object.hpp"
#include "xson/fast/encoder.hpp"
#include "xson/fson/types.hpp"

namespace xson::fson {

class encoder
{
public:

    encoder()
    {}

    void encode(std::ostream& os, const xson::object& o)
    {
        auto type = make_type(o);

        fast::encode(os,type);

        switch(type)
        {
            case type::object:
            for(const auto& [name,value] : o.get<object::map>())
            {
                fast::encode(os,type::name); // type
                fast::encode(os,name);       // name
                encode(os,value);            // object
            }
            fast::encode(os,type::end);
            break;

            case type::array:
            for(const auto& value : o.get<object::array>())
            {
                encode(os,value);            // object
            }
            fast::encode(os,type::end);
            break;

            case type::integer:
            fast::encode(os,std::get<xson::integer_type>(o.get<object::value>()));
            break;

            case type::number:
            fast::encode(os,std::get<xson::number_type>(o.get<object::value>()));
            break;

            case type::string:
            fast::encode(os,std::get<xson::string_type>(o.get<object::value>()));
            break;

            case type::boolean:
            fast::encode(os,std::get<xson::boolean_type>(o.get<object::value>()));
            break;

            case type::timestamp:
            fast::encode(os,std::get<xson::timestamp_type>(o.get<object::value>()));
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
