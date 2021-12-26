#pragma once
#include <cassert>
#include <type_traits>
#include "gsl/not_null.hpp"
#include "xson/fast/decoder.hpp"
#include "xson/builder.hpp"

namespace xson::fson {

class decoder : public fast::decoder
{
public:

    decoder(std::istream& is, gsl::not_null<observer*> o) : fast::decoder{is}, m_observer{o}
    {}

    using fast::decoder::decode;

    void decode()
    {
        auto parent = std::stack<type>{};

        while(m_is)
        {
            xson::string_type name;
            xson::integer_type idx;
            xson::number_type   d;
            xson::string_type str;
            xson::date_type    dt;
            xson::boolean_type  b;
            xson::integer_type  i;

            auto type = xson::type{};
            decode(type);

            switch(type)
            {
                case type::object: // x03
                    parent.push(type::object);
                    m_observer->start_object();
                    break;

                case type::array:  // x04
                    parent.push(type::array);
                    m_observer->start_array();
                    break;

                case type::number: // x01
                    decode(d);
                    m_observer->value(d);
                    break;

                case type::string: // x02
                    decode(str);
                    m_observer->value(str);
                    break;

                case type::boolean: // x08
                    decode(b);
                    m_observer->value(b);
                    break;

                case type::null:    // x0A
                    m_observer->value(nullptr);
                    break;

                case type::date:    // x09
                    decode(dt);
                    m_observer->value(dt);
                    break;

                case type::integer: // x12
                    decode(i);
                    m_observer->value(i);
                    break;

                case type::eod:     // x00
                    parent.pop();
                    if(parent.top() == type::object) m_observer->end_object();
                    if(parent.top() == type::array) m_observer->end_array();
                    break;

                default:
                    assert(false);
                    break;
            }

            if(parent.empty()) return;

            if(parent.top() == type::object)
            {
                decode(name);
                m_observer->name(name);
                continue;
            }

            if(parent.top() == type::array)
            {
                decode(idx);
                m_observer->index(idx);
                continue;
            }
        }
    }

private:

    template<typename T,
             typename = std::enable_if_t<std::is_enum_v<T>>>
    void decode(T& e)
    {
        std::uint8_t byte;
        decode(byte);
        e = static_cast<T>(byte);
        TRACE(e);
    }

    void decode(double& d)
    {
        union {
            std::uint64_t i64;
            xson::number_type d64;
        } i2d;
        decode(i2d.i64);
        d = i2d.d64;
        TRACE(d);
    }

    void decode(bool& b)
    {
        std::uint8_t byte;
        decode(byte);
        b = static_cast<std::uint8_t>(byte);
        TRACE(b);
    }

    void decode(xson::date_type& dt)
    {
        using namespace std::chrono;
        std::uint64_t i64;
        decode(i64);
        dt = system_clock::time_point{milliseconds{i64}};
        TRACE(i64);
    }

    gsl::not_null<observer*> m_observer;

};

} // namespace xson::fson
