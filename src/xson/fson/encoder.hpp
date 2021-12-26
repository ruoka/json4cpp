#pragma once

#include <cassert>
#include <type_traits>
#include "xson/fast/encoder.hpp"
#include "xson/object.hpp"

namespace xson::fson {

class encoder : public fast::encoder
{
public:

    encoder(std::ostream& os) : fast::encoder{os}
    {}

    using fast::encoder::encode;

    void encode(const xson::object& o)
    {
        encode(o.type());

        switch(o.type())
        {
            case type::object:
            for(const auto& [name,value] : o.get<object::map>())
            {
                encode(type::name); // type
                encode(name);       // name
                encode(value);      // object
            }
            encode(type::eod);
            break;

            case type::array:
            for(auto index = 0ull; const auto& value : o.get<object::array>())
            {
                encode(type::index); // type
                encode(index++);     // index
                encode(value);       // object
            }
            encode(type::eod);
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

            case type::date:
            encode(std::get<xson::date_type>(o.get<object::value>()));
            break;

            case type::null:
            break;

            default:
            assert(false);
            break;
        }
    }
};

} // namespace xson::fson
