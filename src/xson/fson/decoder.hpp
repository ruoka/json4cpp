#pragma once

#include <cassert>
#include <type_traits>
#include "xson/object.hpp"
#include "xson/fast/decoder.hpp"

namespace xson::fson {

class decoder : public fast::decoder
{
public:

    decoder(std::istream& is) : fast::decoder(is)
    {}

    using fast::decoder::decode;

    template<typename T,
             typename = std::enable_if_t<std::is_enum_v<T>>>
    void decode(T& e)
    {
        std::uint8_t byte;
        decode(byte);
        e = static_cast<T>(byte);
    }

    void decode(double& d)
    {
        union {
            std::uint64_t i64;
            xson::number_type d64;
        } i2d;
        decode(i2d.i64);
        d = i2d.d64;
    }

    void decode(bool& b)
    {
        std::uint8_t byte;
        decode(byte);
        b = static_cast<std::uint8_t>(byte);
    }

    void decode(xson::date_type& dt)
    {
        using namespace std::chrono;
        std::uint64_t i64;
        decode(i64);
        dt = system_clock::time_point{milliseconds{i64}};
    }

    void decode(object& parent)
    {
        auto type = xson::type{};
        decode(type);

        while(type != type::eod && m_is)
        {
            auto name = ""s;
            decode(name);
            auto& child = parent[name];

            xson::number_type   d;
            xson::string_type str;
            xson::date_type    dt;
            xson::boolean_type  b;
            xson::int32_type  i32;
            xson::int64_type  i64;

            switch(type)
            {
                case type::number: // x01
                decode(d);
                child.value(d);
                break;

                case type::string: // x02
                decode(str);
                child.value(str);
                break;

                case type::object: // x03
                case type::array:  // x04
                decode(child);
                child.type(type);
                break;

                case type::boolean: // x08
                decode(b);
                child.value(b);
                break;

                case type::null:    // x0A
                child.value(nullptr);
                break;

                case type::date:    // x09
                decode(dt);
                child.value(dt);
                break;

                case type::int32:   // x10
                decode(i32);
                child.value(i32);
                break;

                case type::int64:   // x12
                decode(i64);
                child.value(i64);
                break;

                case type::eod:     // x00
                assert(false);
                break;
            }

            decode(type);
        }
    }

};

} // namespace xson::fson

namespace std {

inline auto& operator >> (std::istream& is, xson::object& ob)
{
    xson::fson::decoder{is}.decode(ob);
    return is;
}

} // namespace std
