#pragma once

#include "xson/fast/encoder.hpp"
#include "xson/object.hpp"

namespace xson::fson {

class encoder : public fast::encoder
{
public:

    encoder(std::ostream& os) : fast::encoder{os}
    {}

    using fast::encoder::encode;

    template<typename T>
    std::enable_if_t<std::is_enum<T>::value,void> encode(T e)
    {
        encode(static_cast<std::uint8_t>(e));
    }

    void encode(xson::double_type d)
    {
        union {
            double d64;
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

    void encode(const xson::date_type& d)
    {
        using namespace std::chrono;
        const auto us = duration_cast<milliseconds>(d.time_since_epoch());
        encode(static_cast<std::uint64_t>(us.count()));
    }

    void encode(const xson::object& ob)
    {
        switch(ob.type())
        {
            case type::object:
            case type::array:
            for(const auto& o : ob)
            {
                encode(o.second.type()); // type
                encode(o.first);         // name
                encode(o.second);        // object
            }
            encode(xson::eod);
            break;

            case type::int32:
            encode(static_cast<xson::int32_type>(ob));
            break;

            case type::int64:
            encode(static_cast<xson::int64_type>(ob));
            break;

            case type::number:
            encode(static_cast<xson::double_type>(ob));
            break;

            case type::string:
            encode(static_cast<xson::string_type>(ob));
            break;

            case type::boolean:
            encode(static_cast<xson::boolean_type>(ob));
            break;

            case type::date:
            encode(static_cast<xson::date_type>(ob));
            break;

            case type::null:
            break;
        }
    }

};

} // namespace xson::fson

namespace std {

inline std::ostream& operator << (std::ostream& os, const xson::object& ob)
{
    xson::fson::encoder{os}.encode(ob);
    return os;
}

}
