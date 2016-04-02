#pragma once

#include <cassert>
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
        auto type = xson::type{};
        decode(type);

        while(type != type::eod && m_is)
        {
            auto name = ""s;
            decode(name);
            auto& child = parent[name];

            auto i32 = 0;
            auto i64 = 0ll;
            auto d = 0.0;
            auto str = ""s;
            auto b = false;
            auto tp = std::chrono::system_clock::time_point{};

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

                case type::eod:
                assert(false);
                break;
            }

            decode(type);
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
