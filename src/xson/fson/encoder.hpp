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

    template<typename T,
             typename = std::enable_if_t<std::is_enum_v<T>>>
    void encode(T e)
    {
        encode(static_cast<std::uint8_t>(e));
    }

    void encode(xson::number_type d)
    {
        union {
            std::double_t d64;
            std::uint64_t i64;
        } d2i;
        d2i.d64 = d;
        encode(d2i.i64);
    }

    void encode(xson::boolean_type b)
    {
        if(b)
            encode(std::uint8_t{'\x01'});
        else
            encode(std::uint8_t{'\x00'});
    }

    void encode(const xson::date_type d)
    {
        using namespace std::chrono;
        const auto us = duration_cast<milliseconds>(d.time_since_epoch());
        encode(static_cast<std::uint64_t>(us.count()));
    }

    void encode(const xson::object& o)
    {
        encode(o.type());

        switch(o.type())
        {
            case type::object:
            for(const auto& [name,value] : o.get<object::map>())
            {
                encode(name);         // name
                encode(value);        // object
            }
            encode(type::eod);
            break;

            case type::array:
            for(auto index = 0ull; const auto& value : o.get<object::array>())
            {
                encode(index++);      // index
                encode(value);        // object
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

            case type::eod:
            assert(false);
            break;
        }
    }

};

} // namespace xson::fson
