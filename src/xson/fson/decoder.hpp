#pragma once

#include "xson/object.hpp"
#include "xson/fast/decoder.hpp"

namespace xson::fson {

class decoder : public fast::decoder
{
public:

    decoder(std::istream& is) : fast::decoder(is)
    {}

    using fast::decoder::decode;

    template<typename T>
    std::enable_if_t<std::is_enum<T>::value,void> decode(T& e)
    {
        std::uint8_t byte;
        decode(byte);
        e = static_cast<T>(byte);
    }

    void decode(double& d)
    {
        union {
            std::uint64_t i64;
            double d64;
        } i2d;
        decode(i2d.i64);
        d = i2d.d64;
    }

    void decode(bool& b)
    {
        std::uint8_t byte;
        decode(byte);
        b = static_cast<bool>(byte);
    }

    void decode(std::chrono::system_clock::time_point& tp)
    {
        using namespace std::chrono;
        std::uint64_t i64;
        decode(i64);
        tp = system_clock::time_point{milliseconds{i64}};
    }

    void decode(object& parent)
    {
        std::uint8_t byte;
        decode(byte);

        while(byte != xson::eod && m_is)
        {
            xson::type type = static_cast<xson::type>(byte);
            std::string name;
            decode(name);
            auto& child = parent[name];

            std::int32_t i32;
            std::int64_t i64;
            double d;
            std::string str;
            bool b;
            std::chrono::system_clock::time_point tp;

            switch(type)
            {
                case type::object:
                case type::array:
                decode(child);
                child.type(type);
                break;

                case type::int32:
                decode(i32);
                child.value(i32);
                break;

                case type::int64:
                decode(i64);
                child.value(i64);
                break;

                case type::number:
                decode(d);
                child.value(d);
                break;

                case type::string:
                decode(str);
                child.value(str);
                break;

                case type::boolean:
                decode(b);
                child.value(b);
                break;

                case type::date:
                decode(tp);
                child.value(tp);
                break;

                case type::null:
                child.value(nullptr);
                break;
            }

            decode(byte);
        }
    }

};

} // namespace xson::fson

namespace std {

inline std::istream& operator >> (std::istream& is, xson::object& ob)
{
    xson::fson::decoder{is}.decode(ob);
    return is;
}

} // namespace std
