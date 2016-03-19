#pragma once

#include "xson/fast/encoder.hpp"
#include "xson/object.hpp"

namespace xson {
namespace fson {

class encoder : public fast::encoder
{
public:

    encoder(std::ostream& os) : fast::encoder{os}
    {}

    using fast::encoder::encode;

    void encode(xson::type t)
    {
        encode(static_cast<std::uint8_t>(t));
    }

    void encode(double_type d)
    {
        union {
            double d64;
            std::uint64_t i64;
        } d2i;
        d2i.d64 = d;
        encode(d2i.i64);
    }

    void encode(boolean_type b)
    {
        if(b)
            encode(std::uint8_t{'\x01'});
        else
            encode(std::uint8_t{'\x00'});
    }

    void encode(date_type d)
    {
        using namespace std::chrono;
        const std::int64_t i64 = duration_cast<milliseconds>(d.time_since_epoch()).count();
        encode(i64);
    }

    void encode(const object& ob)
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
            encode(static_cast<int32_type>(ob));
            break;

            case type::int64:
            encode(static_cast<int64_type>(ob));
            break;

            case type::number:
            encode(static_cast<double_type>(ob));
            break;

            case type::string:
            encode(static_cast<string_type>(ob));
            break;

            case type::boolean:
            encode(static_cast<boolean_type>(ob));
            break;

            case type::date:
            encode(static_cast<date_type>(ob));
            break;

            case type::null:
            break;
        }
    }

};

inline std::ostream& operator << (std::ostream& os, const object& ob)
{
    encoder{os}.encode(ob);
    return os;
}

} // namespace fson
} // namespace xson
